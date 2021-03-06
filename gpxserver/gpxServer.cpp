#include "parser.h"
#include "calculator.h"

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

#include "gpxtools.grpc.pb.h"


using grpc::Server;
using grpc::ServerBuilder;


int main(int argc, char* argv[])
{
    std::string server_address("0.0.0.0:50051");
    GpxCalculatorImpl service;
    ParserImpl parser;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&parser);
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
    return 0;
}
