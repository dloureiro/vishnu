#include <iostream>
#include <vector>
#include "zmq.hpp"
#include "zhelpers.hpp"

class LazyPirateClientTest {
public:
  LazyPirateClientTest():ctxt(0), addr("tcp://localhost:5555"){
  }

  ~LazyPirateClientTest(){
  }

  void
  testSend(){
    LazyPirateClient lp(ctxt, addr);
    BOOST_CHECK(lp.send("bonjour"));
  }
  void
  testSendBadAddr(){
    LazyPirateClient lp (ctxt, "bad");
    BOOST_CHECK_THROW(lp.send("bonjour"), std::exception);
  }

  void
  testRecv(){
    LazyPirateClient lp (ctxt, addr);
    lp.send("bonjour");
    BOOST_CHECK(lp.recv().compare("ok") == 0);
  }

  void
  testRecvBadAddr(){
    LazyPirateClient lp (ctxt, "bad");
    BOOST_CHECK_THROW(lp.send("bonjour"), std::exception);
    BOOST_CHECK(lp.recv().compare("ok") !=0 );
  }

private:
  std::string addr;
  zmq::context_t ctxt;
};

