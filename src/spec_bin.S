/*
 * spec_file.s
 *
 *  Created on: Oct 29, 2024
 *      Author: rafael
 */

/*.section .spec_file*/
.section .rodata
.global specs_bin

specs_bin:
	.incbin "./r2u2_spec3.bin"
	.word 2000
