#include "vm.h"

#include <iostream>
#include <limits>

#include "util.h"


namespace vm {


vm_state create_vm(bool debug) {
    vm_state state;

    // enable vm debugging
    state.debug = debug;


    register_instruction(state, "PRINT", [](vm_state& vmstate, const item_t /*arg*/) {
        std::cout << vmstate.stack.top() << std::endl;
        return true;
    });
    // TODO create instructions


    // Load_const Operation
    register_instruction(state, "LOAD_CONST", [](vm_state& vmstate, const item_t item /*arg*/) {
        vmstate.stack.push(item);
        return true;
    });


    // Add Operation
    register_instruction(state, "ADD", [](vm_state& vmstate, const item_t /*arg*/) {
        item_t x = vmstate.stack.top();
        vmstate.stack.pop();
        item_t y = vmstate.stack.top();
        vmstate.stack.pop();

        item_t sum  = x + y;
        vmstate.stack.push(sum);
        return true;
    });

    //Exit Operation
    register_instruction(state, "EXIT", [](vm_state& vmstate, const item_t /*arg*/) {
        item_t x = vmstate.stack.top();
        vmstate.stack.pop();
        return x;
    });


    // register_instruction(state, "LOAD CONST")

    return state;
}


void register_instruction(vm_state& state, std::string_view name,
                          const op_action_t& action) {
    size_t op_id = state.next_op_id;

    // TODO make instruction available to vm
    state.instruction_ids[std::string(name)] = op_id;
    state.instruction_names[op_id] = std::string(name);
    state.instruction_actions[op_id] = action;
    ++state.next_op_id;
}


code_t assemble(const vm_state& state, std::string_view input_program) {
    code_t code;

    // convert each line separately
    for (auto& line : util::split(input_program, '\n')) {

        auto line_words = util::split(line, ' ');

        // only support instruction and one argument
        if (line_words.size() >= 3) {
            throw invalid_instruction{std::string{"more than one instruction argument: "} + line};
        }

        // look up instruction id
        auto& op_name = line_words[0];
        auto find_op_id = state.instruction_ids.find(op_name);
        if (find_op_id == std::end(state.instruction_ids)) {
            throw invalid_instruction{std::string{"unknown instruction: "} + op_name};
        }
        op_id_t op_id = find_op_id->second;

        // parse the argument
        item_t argument{0};
        if (line_words.size() == 2) {
            argument = std::stoll(line_words[1]);
        }

        // and save the instruction to the code store
        code.emplace_back(op_id, argument);
    }

    return code;
}


std::tuple<item_t, std::string> run(vm_state& vm, const code_t& code) {
    // to help you debugging the code!
    if (vm.debug) {
        std::cout << "=== running vm ======================" << std::endl;
        std::cout << "disassembly of run code:" << std::endl;
        for (const auto &[op_id, arg] : code) {
            if (not vm.instruction_names.contains(op_id)) {
                std::cout << "could not disassemble - op_id unknown..." << std::endl;
                std::cout << "turning off debug mode." << std::endl;
                vm.debug = false;
                break;
            }
            std::cout << vm.instruction_names[op_id] << " " << arg << std::endl;
        }
        std::cout << "=== end of disassembly" << std::endl << std::endl;
    }

    // execution loop for the machine
    while (true) {

        auto& [op_id, arg] = code[vm.pc];

        if (vm.debug) {
            std::cout << "-- exec " << vm.instruction_names[op_id] << " arg=" << arg << " at pc=" << vm.pc << std::endl;
        }

        // increase the program counter here so its value can be overwritten
        // by the instruction when it executes!
        vm.pc += 1;

        // TODO execute instruction and stop if the action returns false.
        
    }

    return {0, ""}; // TODO: return tuple(exit value, output text)
}


} // namespace vm
