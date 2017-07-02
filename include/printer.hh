#pragma once

#include <fstream>
#include "cppfsm.hh"

namespace cppfsm
{
  class FlowPrinter : public Listener
  {
    public:
    FlowPrinter(std::string file)
    : link_("->")
    , out_file(file)
    {
      if (!out_file.is_open())
        std::cerr << "Cppfsm: Pretty Printer could not open file: " + file
          << std::endl;
      out_file << "digraph \"Execution flow graph\" {" << std::endl;
      out_file << " node [shape=box];" << std::endl;
    }

    ~FlowPrinter()
    {
      out_file << "}" << std::endl;
    }

    void on_transit(const TransitMessage& m) override
    {
      if (!out_file.is_open()) return;
      out_file << " \"" << m.src_get()
        << "\" " << link_
        << " \"" << m.dst_get()
        << "\"\n";
    }

    void on_entry(const Message& m) override
    {
      if (!out_file.is_open()) return;
      out_file << " \"" << m.state_name_get()
        << "\" [label=\"" << m.state_name_get()
        << "\"]\n";
    }

    void on_exit(const Message& m) override
    {
      //Nothing to print on exit
      if (!out_file.is_open()) return;
      (void)m;
    }

    void on_check(const CheckMessage& m) override
    {
      //Nothing to print on check
      if (!out_file.is_open()) return;
      (void)m;
    }

    private:
    std::string link_;
    std::ofstream out_file;
      
  };
} // namespace cppfsm
