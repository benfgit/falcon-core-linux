Command system
==============

namespace commands

Command sources (cloud, command line, keyboard) implemented by three classes
that derive from CommandSource base class.

Commands are requested (serially) from sources and handled by CommandHandler
class in main thread.

Graph commands are forwarded to GraphManager and handled in graph thread.

Replies are sent to the original requester of the command.
