#ifndef IBANKSERVER_H
#define IBANKSERVER_H

class IBankServer {
public:
    virtual bool isConnected() const = 0;
    virtual ~IBankServer() {}
};

#endif 