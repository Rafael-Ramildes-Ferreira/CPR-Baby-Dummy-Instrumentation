/*
 * spec_file.s
 *
 *  Created on: Oct 29, 2024
 *      Author: rafael
 */

/*.section .spec_file*/
.section .data
.global specs_bin

specs_bin:
	.incbin "./msg.txt"
	.word 0
