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
        if (vmstate.stack.empty()) {
            throw vm::vm_stackfail{"stack fail"};
        }
        std::cout << vmstate.stack.top() << std::endl;
        return true;
    });

    // TODO create instructions
    register_instruction(state, "LOAD_CONST", [](vm_state& vmstate, const item_t arg) {
        vmstate.stack.push(arg);
        return true;
    });

    register_instruction(state, "EXIT", [](vm_state& vmstate, const item_t /*arg*/) {
        return false;
    });

    register_instruction(state, "POP", [](vm_state& vmstate, const item_t /*arg*/) {
        if (vmstate.stack.empty()) {
            throw vm::vm_stackfail{"stack fail"};
        }
        vmstate.stack.pop();
        return true;
    });

    register_instruction(state, "ADD", [](vm_state& vmstate, const item_t /*arg*/) {
        if (vmstate.stack.size() < 2) {
            throw vm::vm_stackfail{"stack fail"};
        }
        item_t tos{vmstate.stack.top()};
        vmstate.stack.pop();
        item_t tos1{vmstate.stack.top()};
        vmstate.stack.pop();
        vmstate.stack.push(tos + tos1);
        return true;
    });

    register_instruction(state, "DIV", [](vm_state& vmstate, const item_t /*arg*/){
        if (vmstate.stack.size() < 2) {
            throw vm::vm_stackfail{"stack fail"};
        }
        item_t tos{vmstate.stack.top()};
        vmstate.stack.pop();
        item_t tos1{vmstate.stack.top()};
        vmstate.stack.pop();
        if (tos == 0) {
            throw vm::div_by_zero{"div by zero"};
        }
        vmstate.stack.push(tos1/tos);
        return true;
    });

    register_instruction(state, "EQ", [](vm_state& vmstate, const item_t /*arg*/){
        if (vmstate.stack.size() < 2) {
            throw vm::vm_stackfail{"stack fail"};
        }
        item_t tos{vmstate.stack.top()};
        vmstate.stack.pop();
        item_t tos1{vmstate.stack.top()};
        vmstate.stack.pop();
        if (tos == tos1) {
            vmstate.stack.push(1);
        } else {
            vmstate.stack.push(0);
        }
        return true;
    });

    register_instruction(state, "NEQ", [](vm_state& vmstate, const item_t /*arg*/){
        if (vmstate.stack.size() < 2) {
            throw vm::vm_stackfail{"stack fail"};
        }
        item_t tos{vmstate.stack.top()};
        vmstate.stack.pop();
        item_t tos1{vmstate.stack.top()};
        vmstate.stack.pop();
        if (tos == tos1) {
            vmstate.stack.push(0);
        } else {
            vmstate.stack.push(1);
        }
        return true;
    });

    register_instruction(state, "DUP", [](vm_state& vmstate, const item_t /*arg*/){
        if (vmstate.stack.empty()) {
            throw vm::vm_stackfail{"stack fail"};
        }
        vmstate.stack.push(vmstate.stack.top());
        return true;
    });

    register_instruction(state, "JMP", [](vm_state& vmstate, const item_t arg){
        vmstate.pc = static_cast<size_t>(arg);
        return true;
    });

    register_instruction(state, "JMPZ", [](vm_state& vmstate, const item_t arg){
        if (vmstate.stack.empty()) {
            throw vm::vm_stackfail{"stack fail"};
        }
        item_t tos{vmstate.stack.top()};
        vmstate.stack.pop();
        if (tos == 0) {
            vmstate.pc = static_cast<size_t>(arg);
        }
        return true;
    });

    register_instruction(state, "WRITE", [](vm_state& vmstate, const item_t /*arg*/){
        if (vmstate.stack.empty()) {
            throw vm::vm_stackfail{"stack fail"};
        }
        vmstate.output_string.append(std::to_string(vmstate.stack.top()));
        return true;
    });

    register_instruction(state, "WRITE_CHAR", [](vm_state& vmstate, const item_t /*arg*/){
        if (vmstate.stack.empty()) {
            throw vm::vm_stackfail{"stack fail"};
        }
        vmstate.output_string.push_back(static_cast<char>(vmstate.stack.top()));
        return true;
    });

    return state;
}


void register_instruction(vm_state& state, std::string_view name,
                          const op_action_t& action) { // why the action is passed by reference? Too big?
    size_t op_id = state.next_op_id; // instruction id

    // TODO make instruction available to vm
    state.instruction_ids.emplace(name, op_id);
    state.instruction_names.emplace(op_id, name);
    state.instruction_actions.emplace(op_id, action);

    state.next_op_id += 1;
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

        if (vm.pc >= code.size()) {
            throw vm::vm_segfault{"non-existent program address"};
        }

        // increase the program counter here so its value can be overwritten
        // by the instruction when it executes!
        vm.pc += 1;

        // TODO execute instruction and stop if the action returns false.
        auto instruaction = vm.instruction_actions[op_id];
        if (instruaction(vm, arg) == false) {
            // stop
            if (vm.stack.empty()) {
                throw vm::vm_stackfail{"stack fail"};
            }
            return {vm.stack.top(), vm.output_string};
        }
    }

    // return {0, ""}; // TODO: return tuple(exit value, output text)
    if (vm.stack.empty()) {
        throw vm::vm_stackfail{"stack fail"};
    }
    return {vm.stack.top(), vm.output_string}; // TODO: return tuple(exit value, output text)

}


} // namespace vm
