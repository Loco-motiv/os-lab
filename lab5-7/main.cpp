#include <iostream>
#include <unistd.h>
#include <string>
#include <optional>
#include <thread>

#include <zmq.hpp>

#include "tree.h"

Node* processes = (Node*)calloc(1, sizeof(Node));

std::string next(std::string* s) {
    size_t i = 0;
    for (; i < s->size() && (*s)[i] != ' ' && (*s)[i] != '\n'; ++i) {}

    std::string result = s->substr(0, i);
    *s = s->substr(i + 1, s->size());

    return result;
}

zmq::context_t ctx;
zmq::socket_t root_socket = zmq::socket_t(ctx, ZMQ_REQ);

void send_msg() {
    zmq::message_t request;
    if (root_socket.recv(request, zmq::recv_flags::none) == -1){
        std::cout << "Err: request error";
    }

    std::string message = std::string(static_cast<char*>(request.data()), request.size());
    std::cout << message << std::endl;
    if (message[0] == 'E'){
        next(&message);
        next(&message);
        node_delete(processes, std::stoi(next(&message)));
    }
}

int main() {
    node_empty(processes);
    std::string line = std::string();
    root_socket.connect("ipc:///tmp/lab5_0");

    pid_t child_pid = fork();

    if (child_pid == -1) {
        std::cout << "Error: Could not fork current process!\n";
        return 1;
    } else if (child_pid == 0) {
        const char* argv[] = { "./calc.out", "ipc:///tmp/lab5_0", NULL};
        if (execv(argv[0], const_cast<char* const*>(argv)) == -1) std::cout << "Error: Could not create a child process!\n";
    }

    while (getline(std::cin, line)) {
        line.push_back('\n');

        std::string command = next(&line);

        if (command == std::string("create")) {
            int child_id = std::stoi(next(&line));
            int parent_id = std::stoi(next(&line));

            if (ping(processes, child_id)) {
                std::cout << "Error: Child with id = " << child_id << " already exists!\n";
            } else if(!ping(processes, parent_id)){
                std::cout << "Error: Parent with id = " << parent_id << " don't exist!\n";
            } else {
                std::string socket_path = "ipc:///tmp/lab5_" + std::to_string(child_id);

                pid_t child_pid = fork();

                if (child_pid == -1) {
                    std::cout << "Error: Could not fork current process!\n";
                } else if (child_pid == 0) {
                    const char *argv[] = { "./calc.out", socket_path.c_str(), NULL};
                    if (execv(argv[0], const_cast<char* const*>(argv)) == -1) std::cout << "Error: Could not create a child process!\n";
                } else {
                    std::cout << "Ok: child pid is " << child_pid << '\n';
                    Node* child = (Node*)calloc(1, sizeof(Node));
                    node_empty(child);
                    child->value = child_id;
                    node_add_children(processes, child, parent_id);
                }
            }
        } else if (command == std::string("exec")) {
            int child_id = std::stoi(next(&line));

            std::string var = next(&line);

            if (line.size() > 0) {
                std::optional<std::vector<int>> path = find_path(processes, child_id);

                if (!path) {
                    std::cout << "Error: No child with id = " << child_id << "!\n";
                    continue;
                }

                std::string value = next(&line);

                std::string msg_command = "s " + var + " " + value + '\n';
                std::string msg;

                for (size_t i = 1; i < path.value().size(); ++i) {
                    int id = path.value()[i];
                    msg += std::to_string(id) + ' ';
                }
                msg += msg_command;

                root_socket.send(zmq::buffer(msg), zmq::send_flags::none);

                std::thread wait(send_msg);
                wait.join();
            } else {
                std::optional<std::vector<int>> path = find_path(processes, child_id);


                if (!path) {
                    std::cout << "Error: No child with id = " << child_id << "!\n";
                    continue;
                }

                std::string msg_command = "g " + var + '\n';
                std::string msg;

                for (size_t i = 1; i < path.value().size(); ++i) {
                    int id = path.value()[i];
                    msg += std::to_string(id) + ' ';
                }
                msg += msg_command;

                root_socket.send(zmq::buffer(msg), zmq::send_flags::none);

                std::thread wait(send_msg);
                wait.join();
            }
        } else if (command == std::string("ping")) {
            int child_id = std::stoi(next(&line));

            std::optional<std::vector<int>> path = find_path(processes, child_id);

            if (!path) {
                std::cout << "Error: No child with id = " << child_id << "!\n";
                std::cout << "> ";
                continue;
            }

            std::string msg_command = "ping\n";
            std::string msg;

            for (unsigned long i = 1; i < path.value().size(); ++i) {
                int id = path.value()[i];
                msg += std::to_string(id) + ' ';
            }
            msg += msg_command;

            root_socket.send(zmq::buffer(msg), zmq::send_flags::none);
            
            std::thread wait(send_msg);
            wait.join();

        } else if (command == std::string("tree")){
            node_print_tree(processes);
        } else if (command == std::string("exit")) {
            break;
        }
    }

    return 0;
}