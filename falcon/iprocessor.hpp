// ---------------------------------------------------------------------
// This file is part of falcon-core.
// 
// Copyright (C) 2015, 2016, 2017 Neuro-Electronics Research Flanders
// 
// Falcon-server is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Falcon-server is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with falcon-core. If not, see <http://www.gnu.org/licenses/>.
// ---------------------------------------------------------------------

#ifndef IPROCESSOR_H
#define IPROCESSOR_H

#include <string>
#include <set>
#include <memory>

#include "threadutilities.hpp"
#include "runinfo.hpp"
#include "portpolicy.hpp"

#include "streamports.hpp"

#include "yaml-cpp/yaml.h"
#include "factory/factory.hpp"

#include "graphexceptions.hpp"

#include <functional>
#include "sharedstate.hpp"

#include "options/options.hpp"


// exception class for all processor related errors
GRAPHERROR( ProcessorInternalError );
GRAPHERROR( ProcessingError );
GRAPHERROR( ProcessingConfigureError );
GRAPHERROR( ProcessingCreatePortsError );
GRAPHERROR( ProcessingStreamInfoError );
GRAPHERROR( ProcessingPrepareError );
GRAPHERROR( ProcessingPreprocessingError );

bool is_valid_name( std::string s );

namespace graph {
class ProcessorGraph;
}

class IProcessor {

friend class ISlotIn;
friend class graph::ProcessorGraph;

public: // called by anyone
    IProcessor( ThreadPriority priority = PRIORITY_NONE ) :
    running_(false), thread_() 
    {
        // add test option
        add_option("test", new_test_flag_);

        // add advanced options
        add_advanced_option("threadcore", thread_core_);
        add_advanced_option("threadpriority", thread_priority_=priority);
        add_advanced_option("buffer_sizes", requested_buffer_sizes_);

    }

    ~IProcessor() { internal_Stop(); }

    const std::string name() const { return name_; }
    const std::string type() const { return type_; }
    
    unsigned int n_input_ports() const { return input_ports_.size(); }
    unsigned int n_output_ports() const { return output_ports_.size(); }
    
    const std::set<std::string> input_port_names() const; 
    const std::set<std::string> output_port_names() const;

    bool has_input_port( std::string port ) { return input_ports_.count( port )==1; }
    bool has_output_port( std::string port ) { return output_ports_.count( port )==1; }
    
    virtual bool issource() const { return n_input_ports()==0; }
    virtual bool issink() const { return n_output_ports()==0; }
    virtual bool isfilter() const { return (!issource() && !issink()); }
    virtual bool isautonomous() const { return (issource() && issink()); }
    
    ThreadPriority thread_priority() const { return thread_priority_(); }
    ThreadCore thread_core() const { return thread_core_(); }
    
    bool running() const { return running_.load(); };
   
    YAML::Node ExportYAML();

protected: // need to be removed??
    std::map<std::string, std::shared_ptr<std::ostream>> streams_;
    std::vector<TimePoint> test_source_timestamps_;

    /* this methods creates a file whose access key is filename and whose
    fullpath is prefix.filename.extension*/
    void create_file( std::string prefix, std::string variable_name,
        std::string extension="bin" );
    
    void prepare_latency_test( ProcessingContext& context );
    void save_source_timestamps_to_disk( std::uint64_t n_timestamps );

protected: // callable by derived processors, but not others
    
    template <typename TValue>
    void add_option(std::string name, TValue & value, std::string description="", bool required=false) {
        options_.add(name, value, description, required);
    }

    void remove_option(std::string name);

    template <typename DATATYPE>
    PortOut<DATATYPE>* create_output_port(
        const typename DATATYPE::Capabilities & capabilities,
        const typename DATATYPE::Parameters & parameters,
        const PortOutPolicy& policy ) {
        
        return create_output_port<DATATYPE>(
            DATATYPE::dataname(), capabilities, parameters, policy);
    }

    template <typename DATATYPE>
    PortOut<DATATYPE>* create_output_port( std::string name,
                                           const typename DATATYPE::Capabilities & capabilities,
                                           const typename DATATYPE::Parameters & parameters,
                                           const PortOutPolicy& policy ) {
        
        if (name.size()==0) { name = DATATYPE::dataname(); }

        if (output_ports_.count( name )==1 || !is_valid_name(name)) {
            throw std::runtime_error( "Output port name \"" + name + "\" is invalid or already exists." );
        }
        
        output_ports_[name] = std::move(
            std::unique_ptr<IPortOut>(
                (IPortOut*) new PortOut<DATATYPE>( this,
                                                   PortAddress(this->name(),name),
                                                   capabilities,
                                                   parameters,
                                                   policy ) ) );
        
        return ((PortOut<DATATYPE>*) output_ports_[name].get());
    }
    
    template <typename DATATYPE>
    PortIn<DATATYPE>* create_input_port( 
        const typename DATATYPE::Capabilities & capabilities,
        const PortInPolicy & policy) {
        
        return create_input_port<DATATYPE>(
            DATATYPE::dataname(), capabilities, policy);
    }

    template <typename DATATYPE>
    PortIn<DATATYPE>* create_input_port( std::string name,
                                         const typename DATATYPE::Capabilities & capabilities,
                                         const PortInPolicy & policy ) {
        
        if (name.size()==0) { name = DATATYPE::dataname(); }

        if (input_ports_.count( name )==1 || !is_valid_name(name)) {
            throw std::runtime_error( "Input port name \"" + name + "\" is invalid or already exists." );
        }
        
        input_ports_[name] = std::move(
            std::unique_ptr<IPortIn>(
                (IPortIn*) new PortIn<DATATYPE>( this,
                                                 PortAddress(this->name(),name),
                                                 capabilities,
                                                 policy ) ) );
        
        return ((PortIn<DATATYPE>*) input_ports_[name].get());
    }
    
    IPortIn* input_port( std::string port ) { return input_ports_.at(port).get(); }
    IPortOut* output_port( std::string port ) { return output_ports_.at(port).get(); }

    IPortIn* input_port( const PortAddress & address );
    IPortOut* output_port( const PortAddress & address );    

    ISlotIn* input_slot( const SlotAddress & address );
    ISlotOut* output_slot( const SlotAddress & address );



    /*self – peers – external

    static variable: value is not changed by self or others
    R – N – N (static variable) -> not very useful
    R – N – R (externally observed static variable)
    R – R – N (static shared observable)
    R – R – R (externally observed static shared observable)
    R – N – W (externally controlled static variable)
    R – R – W (externally controlled shared static variable)
    isolated producer: value is changed by self only
    W – N – N (isolated producer) -> not very useful
    W – N – R (externally observed isolated producer)
    W – N – W (bi-directional channel)
    co-operative producer: self and others change the value
    W – W – N (co-operative producer)
    W – W – R (externally observed co-operative producer)
    W – W – W (externally controlled co-operative producer)
    follower: value is changed by others, self only reads
    R – W – N (follower)
    R – W – R (externally observed follower)
    R – W – W (externally controlled follower)
    broadcaster: self changes the value, others follow
    W – R – N (broadcaster)
    W – R – R (externally observed broadcaster)
    W – R – W (externally controlled broadcaster)

    */

    template <typename T>
    StaticState<T>* create_static_state(
      std::string state, T default_value, bool shared = true,
      Permission external = Permission::READ, std::string description = "" ) {
        if( shared ){
            return ((StaticState<T>*) create_readable_shared_state(state, default_value, Permission::READ, external, description));
        }
        else {
            return ((StaticState<T>*) create_readable_shared_state(state, default_value, Permission::NONE, external, description));
        }
    }

    template <typename T>
    ProducerState<T>* create_producer_state(
      std::string state, T default_value, bool cooperative = false, Permission external = Permission::READ, std::string description = "" ) {
        if( cooperative ){
            return ((ProducerState<T>*) create_writable_shared_state(state, default_value, Permission::WRITE, external, description));
        }
        else{
            return ((ProducerState<T>*) create_writable_shared_state(state, default_value, Permission::NONE, external, description));
        }
    }

    template <typename T>
    BroadcasterState<T>* create_broadcaster_state(
      std::string state, T default_value, Permission external = Permission::NONE, std::string description = "" ) {
        return ((BroadcasterState<T>*)  create_writable_shared_state(state, default_value, Permission::READ, external, description));
    }

    template <typename T>
    FollowerState<T>* create_follower_state(
      std::string state, T default_value, Permission external = Permission::NONE, std::string description = "" ) {
        return ((FollowerState<T>*) create_readable_shared_state(state, default_value, Permission::WRITE, external, description));
    }

    
    template <typename T>
    ReadableState<T>* create_readable_shared_state(
      std::string state, Permission peers = Permission::WRITE,
      Permission external = Permission::NONE, std::string description = "" ) {
        
        T default_value = T();

        if (!options_.has_option(state)) {
            // use default constructed value
        } else {
            try {
                default_value = dynamic_cast<options::Option<T>&>(options_[state]).get_value();
            } catch (std::runtime_error & e) {
                throw ProcessorInternalError("Could not set state value from option: " + std::string(e.what()));
            }
        }

        return create_readable_shared_state<T>(state, default_value, peers, external, description);
    }

    template <typename T>
    ReadableState<T>* create_readable_shared_state(
      std::string state, T default_value, Permission peers = Permission::WRITE,
      Permission external = Permission::NONE, std::string description = "" ) {
        if (shared_states_.count( state)==1 || !is_valid_name(state)) {
            throw ProcessorInternalError( "Shared state \"" + state + "\" is invalid or already exists.", name() );
        }

        shared_states_[state] = std::move( std::unique_ptr<IState>( (IState*) new ReadableState<T>( default_value, description, peers, external ) ) );

        return ((ReadableState<T>*) shared_states_[state].get());
    }
    
    template <typename T>
    WritableState<T>* create_writable_shared_state(
      std::string state, Permission peers = Permission::READ,
      Permission external = Permission::NONE, std::string description = "" ) {
        
        T default_value = T();

        if (!options_.has_option(state)) {
            // use default constructed value
        } else {
            try {
                default_value = dynamic_cast<options::Option<T>&>(options_[state]).get_value();
            } catch (std::runtime_error & e) {
                throw ProcessorInternalError("Could not set state value from option: " + std::string(e.what()));
            }
        }

        return create_writable_shared_state<T>(state, default_value, peers, external, description);
    
    }

    template <typename T>
    WritableState<T>* create_writable_shared_state(
      std::string state, T default_value, Permission peers = Permission::READ,
      Permission external = Permission::NONE, std::string description = "" ) {
        if (shared_states_.count( state)==1 || !is_valid_name(state)) {
            throw ProcessorInternalError( "Shared state \"" + state + "\" is invalid or already exists.", name() );
        }
        
        shared_states_[state] = std::move( std::unique_ptr<IState>( (IState*) new WritableState<T>( default_value, description, peers, external ) ) );
        
        return ((WritableState<T>*) shared_states_[state].get());
    }
    
    std::shared_ptr<IState> shared_state(std::string state) {
        if (this->shared_states_.count(state)==0) {
            throw ProcessorInternalError( "Shared state \"" + state + "\" does not exist.", name() );
        }
        return shared_states_[state];
    }
    
    template <class T>
    void expose_method( std::string methodname, YAML::Node (T::*method)(const YAML::Node&) ) {
        if (exposed_methods_.count( methodname )==1 || !is_valid_name(methodname)) {
            throw ProcessorInternalError( "Exposed method \"" + methodname + "\" is invalid or already exists.", name() );
        }
        exposed_methods_[methodname] = std::bind( method, static_cast<T*>(this), std::placeholders::_1 );
    }
    
    std::function<YAML::Node(const YAML::Node&)>& exposed_method(std::string method) {
        if (this->exposed_methods_.count(method)==0) {
            throw ProcessorInternalError( "Exposed method \"" + method + "\" does not exist.", name() );
        }
        return exposed_methods_[method];
    }
    
protected: // to be overridden and callable by derived processors
    virtual std::string default_input_port() const;
    virtual std::string default_output_port() const;
    
private: // to be overridden by derived processors, callable internally
    virtual void Configure(const YAML::Node& node, const GlobalContext& context) {};
    virtual void CreatePorts() = 0;
    virtual void Preprocess( ProcessingContext& context ) {};
    virtual void Process( ProcessingContext& context ) = 0;
    virtual void Postprocess( ProcessingContext& context ) {};
    virtual void CompleteStreamInfo();
    virtual void Prepare( GlobalContext& context ) {};
    virtual void Unprepare( GlobalContext& context ) {};
    virtual void TestPrepare( ProcessingContext& context ) {};
    virtual void TestFinalize( ProcessingContext& context ) {};

private: // callable internally only

    void internal_Configure(const YAML::Node& node, const GlobalContext& context); // from engine
    void internal_CreatePorts();    
    void internal_PrepareConnectionIn( SlotAddress & in );
    void internal_PrepareConnectionOut( SlotAddress & out );
    void internal_ConnectionCompatibilityCheck( const SlotAddress & address, IProcessor * upstream, const SlotAddress & upstream_address );
    void internal_ConnectIn( const SlotAddress & address, IProcessor * upstream, const SlotAddress & upstream_address);
    void internal_ConnectOut( const SlotAddress & address, IProcessor * downstream, const SlotAddress & downstream_address);
    
    void internal_NegotiateConnections();
    
    void internal_CreateRingBuffers();
    void internal_PrepareProcessing();
    
    void internal_ThreadEntry(RunContext& runcontext);
    
    void internal_Start(RunContext& runcontext);
    void internal_Stop();
    
    void internal_Alert();    
    
    YAML::Node internal_ApplyMethod( std::string name, const YAML::Node& node );    
    
    void set_name_and_type( std::string name, std::string type ) {
        name_ = name;
        type_ = type;
    }

    template <typename TValue>
    void add_advanced_option(std::string name, TValue & value, std::string description="", bool required=false) {
        advanced_options_.add(name, value, description, required);
    }

private: // member variables
    std::string name_;
    std::string type_;

    std::map<std::string, std::unique_ptr<IPortIn>> input_ports_;
    std::map<std::string, std::unique_ptr<IPortOut>> output_ports_;
    
    std::map<std::string,std::function<YAML::Node(const YAML::Node&)>> exposed_methods_;
    std::map<std::string,std::shared_ptr<IState>> shared_states_;

    bool negotiated_ = false;
    bool prepared_ = false;
    
    std::atomic<bool> running_;
            
    std::thread thread_;

    options::Value<ThreadPriority,false> thread_priority_{
        PRIORITY_NONE,
        options::inrange<ThreadPriority>(PRIORITY_NONE, PRIORITY_HIGH)
    };

    options::Value<ThreadCore,false> thread_core_{
        CORE_NOT_PINNED,
        options::inrange<ThreadCore>(
            CORE_NOT_PINNED,
            (ThreadCore) sysconf(_SC_NPROCESSORS_ONLN)-1
        )
    };

    options::NullableBool new_test_flag_;
    options::Value<std::map<std::string,int>> requested_buffer_sizes_{};

protected:
    options::OptionList options_;
    options::OptionList advanced_options_;

};

typedef std::map<std::string, std::pair< std::string, std::unique_ptr<IProcessor>>> ProcessorMap;

typedef factory::ObjectFactory<IProcessor, std::string> ProcessorFactory;

template<class PROCESSOR>
class ProcessorRegistrar {
public:
    ProcessorRegistrar(std::string name);
};

template <class PROCESSOR>
ProcessorRegistrar<PROCESSOR>::ProcessorRegistrar(std::string name) {
    ProcessorFactory::instance().registerClass( name, factory::createInstance<IProcessor, PROCESSOR> );
}

#define REGISTERPROCESSOR(PROCESSOR) \
    namespace { \
        static ProcessorRegistrar<PROCESSOR> _registrar( #PROCESSOR ); \
    };

#endif
