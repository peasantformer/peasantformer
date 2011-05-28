#ifndef PEASANTFORMER_Data_Connections
#define PEASANTFORMER_Data_Connections

/// @file

//#include <Proto/Network/NetworkHighLevel.h>
//#include <Proto/PlainIO/PlainIOHighLevel.h>
//#include <Proto/Protocol/Protocol.h>
//#include <Proto/Messages/Messages.h>


/// Basic connection class.
class Connection {

};

/// Pending connection class.
class ConnectionPending : public Connection {

};

/// Accepted connection class.
class ConnectionAccepted : public ConnectionPending {

};

#endif
