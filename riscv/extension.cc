// See LICENSE for license details.

#include "extension.h"
#include "trap.h"

extension_t::~extension_t()
{
}

void extension_t::illegal_instruction()
{
  throw trap_illegal_instruction(0);
}

void extension_t::raise_interrupt()
{
  p->take_interrupt((reg_t)1 << IRQ_COP); // must not return
  throw std::logic_error("a COP exception was posted, but interrupts are disabled!");
}

void extension_t::clear_interrupt()
{
}

void xs_gatherer_t::reset() {
  for (auto e : custom_extensions) {
    e.second->reset();
  }
}

void xs_gatherer_t::register_extension(extension_t* x)
{
  for (auto insn : x->get_instructions())
    p->register_insn(insn);
  p->build_opcode_map();

  for (auto disasm_insn : x->get_disasms())
    p->disassembler->add_insn(disasm_insn);

  if (!custom_extensions.insert(std::make_pair(x->name(), x)).second) {
    fprintf(stderr, "extensions must have unique names (got two named \"%s\"!)\n", x->name());
    abort();
  }
  x->set_processor(p);
}
