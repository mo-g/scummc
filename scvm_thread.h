/* ScummC
 * Copyright (C) 2006  Alban Bedel
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

typedef struct scvm_script {
  unsigned id;
  unsigned size;
  unsigned char code[0];
} scvm_script_t;

// thread state
#define SCVM_THREAD_STOPPED 0
#define SCVM_THREAD_RUNNING 1
#define SCVM_THREAD_PENDED  2
#define SCVM_THREAD_DELAYED 3
#define SCVM_THREAD_FROZEN  4

// thread flags
#define SCVM_THREAD_NO_FREEZE 1
#define SCVM_THREAD_RECURSIVE 2

typedef struct scvm_thread scvm_thread_t;

struct scvm_thread {
  unsigned id;
  // unused, running, pended, delayed, frozen
  unsigned state;
  // used for script runned by the VM itself
  unsigned next_state;
  // parent thread for recursive calls
  scvm_thread_t* parent;
  unsigned flags;
  unsigned cycle;
  // script beeing run
  scvm_script_t* script;
  // position in the code
  unsigned code_ptr;
  // thread variables
  unsigned num_var;
  int *var;
};

typedef int (*scvm_op_f)(struct scvm* vm, scvm_thread_t* thread);

typedef struct scvm_op {
  scvm_op_f op;
  char* name;
} scvm_op_t;

int scvm_thread_r8(scvm_thread_t* thread, uint8_t* ret);

int scvm_thread_r16(scvm_thread_t* thread, uint16_t* ret);

int scvm_thread_r32(scvm_thread_t* thread, uint32_t* ret);

int scvm_thread_strlen(scvm_thread_t* thread, unsigned* len);

int scvm_start_thread(scvm_t* vm, scvm_script_t* scr, unsigned code_ptr,
                      unsigned flags, unsigned* args);

int scvm_stop_thread(scvm_t* vm, scvm_thread_t* thread);

int scvm_thread_do_op(scvm_t* vm, scvm_thread_t* thread, scvm_op_t* optable);

int scvm_thread_run(scvm_t* vm, scvm_thread_t* thread);

int scvm_start_script(scvm_t* vm, unsigned flags, unsigned num, unsigned* args);

int scvm_stop_script(scvm_t* vm, unsigned id);