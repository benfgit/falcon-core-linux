#ifndef FACTORY_H
#define FACTORY_H

#include <map>
#include <vector>
#include <utility>
//#include <stdexcept>
#include <exception>
#include <iostream>
#include "yaml-cpp/yaml.h"

namespace factory {
    
// exceptions
class UnknownClass : public std::runtime_error {
public:
    UnknownClass( std::string const & error ) : std::runtime_error(error) {}
};
class DuplicateClass : public std::runtime_error {
public:
    DuplicateClass( std::string const & error ) : std::runtime_error(error) {}
};

template <typename AbstractObject, typename ...Args>
using ObjectCreator = AbstractObject* (*) ( Args&& ... );

template <typename AbstractObject,
          typename IdentifierType,
          typename ...Args >
class ObjectFactory {
    typedef ObjectFactory<AbstractObject, IdentifierType, Args...> ThisClass;
    
public:
    
    AbstractObject * create(const IdentifierType & id, Args ...args) {       
        typename ObjectMap::const_iterator i = this->objectmap_.find(id);
        
        if (this->objectmap_.end() != i) {
            return (i->second)(std::forward<Args>(args)...);
        }
        //return (AbstractObject*)NULL;
        throw UnknownClass( "Cannot create object of unregistered class." );
    }
    
    bool hasClass(const IdentifierType & id) {
        return this->objectmap_.find(id) != this->objectmap_.end();
    } 
    
    bool registerClass(const IdentifierType & id, ObjectCreator<AbstractObject, Args...> creator, YAML::Node doc) {
        if (this->objectmap_.find(id) != this->objectmap_.end()) { 
            throw DuplicateClass( "Cannot register the same class twice." );
        }
        this->docmap_.insert(typename DocMap::value_type(id, doc)).second;
        return this->objectmap_.insert(typename ObjectMap::value_type(id, creator)).second;
    }
    
    static ThisClass& instance() {
        static ThisClass factory;
        return factory;
    }
    
    std::vector<IdentifierType> listEntries( ) const {
        std::vector<IdentifierType> entries;
        for (auto imap: objectmap_ ) {
            entries.push_back( imap.first);
        }
        return entries;
    }

    YAML::Node listDocs(std::string id) const {
        std::vector<YAML::Node> docs;
        auto imap = docmap_.find(id);
        return imap->second;
    }

private:
    typedef std::map<IdentifierType, ObjectCreator<AbstractObject,Args...>> ObjectMap;
    typedef std::map<IdentifierType, YAML::Node> DocMap;
    ObjectMap objectmap_;
    DocMap docmap_;
};

template <typename Base, typename Derived, typename ...Args>
Base * createInstance( Args&& ...args ) {
    return new Derived( std::forward<Args>(args)... ) ;
}



} // namespace factory

#endif // FACTORY_H
