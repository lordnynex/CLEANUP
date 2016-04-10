#ifndef ADVANCEDQUEUE_H
#define ADVANCEDQUEUE_H

#include <deque>
#include <boost/thread/xtime.hpp>

template < class StorageType >
struct WeigthedMsg {
    WeigthedMsg( const StorageType msg, int weight = 1 ): message(msg), message_weigth( weight ) {}
    StorageType message;
    int message_weigth;
};

// AdvancedQueueInput Definition
template < class StorageType >
class AdvancedQueueInput {
public:
    AdvancedQueueInput( int buff_size );
    bool receive_msg( const WeigthedMsg< StorageType > );
    virtual bool before_msg_received( const WeigthedMsg< StorageType >& ) { return true; }
    virtual void on_msg_received( const WeigthedMsg< StorageType >& ) {}
private:
    int slots_used;
    int slots_total;
    std::deque< WeigthedMsg< StorageType > > slots_storage;
    friend class AdvancedQueue;
};
// AdvancedQueueInput Implementation
template < class StorageType >
bool AdvancedQueueInput<StorageType>::AdvancedQueueInput( int buff_size ) {
    slots_total = buff_size;
    slots_used = 0;
}

template < class StorageType >
bool AdvancedQueueInput<StorageType>::receive_msg( const WeigthedMsg msg ) {
    if ( msg.message_weigth+ slots_used > slots_total ) {
        return false;
    }
    if ( !before_msg_received( msg ) )
        return false;

    slots_storage.push_back( msg );
    slots_used += msg.message_weigth;
    on_msg_received( msg );
}
// AdvancedQueueInput Done

template < class StorageType >
class AdvancedQueueOutput {
public:
    AdvancedQueueOutput( double throughput );
    virtual void msg_received() = 0;
private:
    boost::xtime last_sent;
};

template < class StorageType, class PriorityManager >
class AdvancedQueue {
public:
    void processAll();

private:
    void connect( AdvancedQueueInput< StorageType >& qin );
    void connect( AdvancedQueueOutput< StorageType >& qout );

    friend class AdvancedQueueInput;
    friend class AdvancedQueueOutput;
};


#endif // ADVANCEDQUEUE_H
