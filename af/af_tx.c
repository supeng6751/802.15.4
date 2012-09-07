/*******************************************************************
    
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
 
    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Neither the name of the the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.
    4. This software is subject to the additional restrictions placed on the
       Zigbee Specification's Terms of Use.
    
    THIS SOFTWARE IS PROVIDED BY THE THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
    OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
 

*******************************************************************/
/*!
    \file af_tx.c
    \ingroup af
    \brief AF tx queue

    These functions implement the TX queue for the Application Framework.
*/
/**************************************************************************/
#include "freakz.h"

/**************************************************************************/
/*!
        List head for the AF TX queue. Queues up frames for transmission and provides a break
        in the transmit path so that it won't hog the MCU time.
*/
/**************************************************************************/
LIST(af_tx_queue);

/**************************************************************************/
/*!
    Init the TX queue.
*/
/**************************************************************************/
void af_tx_init()
{
    list_init(af_tx_queue);
}

/**************************************************************************/
/*!
    Find a free entry and add it to the TX queue.
*/
/**************************************************************************/
static mem_ptr_t *af_tx_alloc()
{
    mem_ptr_t *mem_ptr;

    if ((mem_ptr = mem_heap_alloc(sizeof(af_tx_entry_t))) != NULL)
    {
        list_add(af_tx_queue, mem_ptr);
    }
    return mem_ptr;
}

/**************************************************************************/
/*!
    Remove the entry from the TX queue and free it.
*/
/**************************************************************************/
void af_tx_free(mem_ptr_t *mem_ptr)
{
    if (mem_ptr)
    {
        list_remove(af_tx_queue, mem_ptr);
        mem_heap_free(mem_ptr);
    }
}

/**************************************************************************/
/*!
    Allocate and add an entry to the TX queue and use the specified args
    to fill it out.
*/
/**************************************************************************/
void af_tx_add(buffer_t *buf, U8 src_ep, U16 dest_addr, U8 dest_ep, U16 clust, U16 prof_id, U8 mode, U8 tx_opt, U8 radius, U8 handle)
{
    mem_ptr_t *mem_ptr;

    if ((mem_ptr = af_tx_alloc()) != NULL)
    {
        TX_ENTRY(mem_ptr)->buf          = buf;
        TX_ENTRY(mem_ptr)->src_ep       = src_ep;
        TX_ENTRY(mem_ptr)->dest_addr    = dest_addr;
        TX_ENTRY(mem_ptr)->dest_ep      = dest_ep;
        TX_ENTRY(mem_ptr)->clust        = clust;
        TX_ENTRY(mem_ptr)->prof_id      = prof_id;
        TX_ENTRY(mem_ptr)->mode         = mode;
        TX_ENTRY(mem_ptr)->tx_opt       = tx_opt;
        TX_ENTRY(mem_ptr)->radius       = radius;
        TX_ENTRY(mem_ptr)->handle       = handle;
    }
}

/**************************************************************************/
/*!
    Pop an entry off the TX queue.
*/
/**************************************************************************/
mem_ptr_t *af_tx_pop()
{
    return list_head(af_tx_queue);
}

