class Server {
    public:
        void listen_and_serve();

        virtual handle(int conn) = 0;
};

struct DumbServer : public Server {
    std::vector<int> conns;
    virtual void handle(int conn) { conns.push_back(conn); }
};

TEST(Server, AcceptsConnection) {
    DumbServer s;
    s.listen_and_serve;

    ASSERT_EQ(1, s.conns.size())
}