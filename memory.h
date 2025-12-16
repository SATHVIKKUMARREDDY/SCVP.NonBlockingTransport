#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <tlm.h>
#include <systemc.h>

template < uint64_t T = 1024 >  // which is the best practice for template parameter naming? size_t : uint64_t
// TODO: add your code here
class memory : public sc_core::sc_module, public tlm::tlm_fw_transport_if<>
{
    private:
        unsigned char mem[T];
    public:
    tlm::tlm_target_socket<> tSocket;       
    SC_CTOR(memory) : tSocket("tSocket")
    {
        tSocket.bind(*this);
    }       
    // Implement b_transport method
    void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay)
    {
        uint64_t addr = trans.get_address();
        unsigned char* ptr = trans.get_data_ptr();  
        if(addr >= 1024)
        {
            trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
            return; 
        }
        if(trans.get_command() == tlm::TLM_READ_COMMAND)
        {
            memcpy(ptr,
                  &mem[addr],
                  trans.get_data_length());
        }
        else //(trans.get_command() == tlm::TLM_WRITE_COMMAND)
        {
            memcpy(&mem[addr],
                  ptr,
                  trans.get_data_length());   
        }

        delay += sc_core::sc_time(20, sc_core::SC_NS);

        trans.set_response_status(tlm::TLM_OK_RESPONSE);
    }

    // Dummy method
    virtual tlm::tlm_sync_enum nb_transport_fw(
            tlm::tlm_generic_payload& trans,
            tlm::tlm_phase& phase,
            sc_time& delay )
    {
        SC_REPORT_FATAL(this->name(),"nb_transport_fw is not implemented");
        return tlm::TLM_ACCEPTED;
    }

    // Dummy method
    bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans,
                            tlm::tlm_dmi& dmi_data)
    {
        SC_REPORT_FATAL(this->name(),"get_direct_mem_ptr is not implemented");
        return false;
    }

    // Dummy method
    unsigned int transport_dbg(tlm::tlm_generic_payload& trans)
    {
        SC_REPORT_FATAL(this->name(),"transport_dbg is not implemented");
        return 0;
    }

};


#endif // MEMORY_H
