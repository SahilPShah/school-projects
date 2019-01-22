# mp1.S - missile-command version
# Good luck, and have fun!
# SAHIL SHAH
# sahils2
# 9/17/18
.data

    # Constants for accessing the fields of a struct missile,
    # struct missile is defined in rtc.h and mp1.h

	NEXT 	  = 0
	X	  = 4
	Y	  = 8
	VX	  = 12
	VY	  = 16
	DEST_X	  = 20
	DEST_Y	  = 24
	EXPLODED  = 28
	C	  = 32

    # Character to draw for an explosion - '@'
	EXPLOSION = 64
	SPACE = 32
	# character to be drawn for the crosshairs - '+'
	XHAIR = 88
	# command values for the dispatcher
	STARTGAME_CMD = 0
	ADDMISSILE_CMD = 1
	MOVEXHAIRS_CMD = 2
	STATUS_CMD = 3
	ENDGAME_CMD = 4
	CENTER_X = 40
	CENTER_Y = 12
	HIGHHALF_SHIFT = 16
	XMAX = 80
	YMAX = 25
	BYTESPERCHAR = 2
	BYTESPERROWCHAR = 160			# 80 chars/row * 2 Bytes/char
	BITMASK = 0x0000FFFF			# bitmask the bottom half
	BASE1_SELECT = 0
	BASE2_SELECT = 1
	BASE3_SELECT = 2
	BASE1_X = 18					# starting columns for each base
	BASE2_x = 38
	BASE3_x = 58
	MISSILE_SIZE = 33				# size of missile explode
	BASECHAR_SEL0 = 0				# character selects for each base
	BASECHAR_SEL1 = 1
	BASECHAR_SEL2 = 2
	BASECHAR_SEL3 = 3
	BASECHAR_SEL4 = 4
	BASE1_OFFSET = 3876				# offsets into video memory for each base
	BASE2_OFFSET = 3916
	BASE3_OFFSET = 3956
	UNSIGNED_LONG_PTR_SIZE = 4
	BASE1_ALIVE = 0x00010000		# 1 in bit 16
	BASE2_ALIVE = 0x00020000		# 1 in bit 17
	BASE3_ALIVE = 0x00040000		# 1 in bit 18
	LAST_BASE_CHAR = 0x5C
	SHIFT8 = 8						# shift 8 bits
# Data shared between this file and rtc.c helper functions

# This '.globl' directive makes these symbols visible externally
.globl mp1_missile_list, base_alive, mp1_score

missiles_in_list:	.long 0x0
mp1_missile_list:	.long 0x0	# Head of list pointer Null in x86 is just 0x00
base_alive:		.long 0x0	# Array of 3 bytes, plus a padding byte
mp1_score:		.long 0x0	# Player's current score

previous_missile: 	.long 0x0	# pointer keeps track of previous missile
# Data private to this  file

base_pic:     	    .string "/^^^\\"	# Picture of a live base
dead_base_pic: 	    .string "xxxxx"	# Picture of a dead base
crosshairs_x:		.long 0x0	# X-position of the crosshairs
crosshairs_y:		.long 0x0	# Y-position of the crosshairs

.text

# void mp1_poke(void);
# You can use this function to write to video memory.
#
# Interface: Register-based arguments (not C-style)
#    Inputs: %cl  - The byte you wish to write
#            %eax - Offset from the start of video memory that you wish
#                   to write to
#   Outputs: Text-mode video screen is written to at location %eax with
#            the byte in %cl
# Registers: Clobbers EDX

mp1_poke:
		movl    vmem_base_addr(,1),%edx
		movb    %cl,(%edx,%eax,1)
		ret

#   ----------------- Exported functions ---------------------

# void mp1_rtc_tasklet(unsigned long garbage);
# Performs three tasks:
#	(1) updates the list of missiles (implement this in update_missiles,
#           below, and call it from here).
#	(2) Redraw the bases - they may have been overwritten with missiles
#	(3) Redraw the crosshairs - it may have been overwritten with missiles
# Inputs   : none
# Outputs  : none
# Registers: Standard C calling convention

.globl mp1_rtc_tasklet
mp1_rtc_tasklet:
		# save, ecx, ebx, eax, edx, esi, edi
		pushl %ebp
		movl %esp, %ebp
		pushl %ebx
		pushl %esi
		pushl %edi
		pushl %eax
		pushl %edx
		pushl %ecx

		movl mp1_missile_list, %ebx
	CHECK_PTR:
		cmpl $0x0, %ebx
		je NULL
		pushl %ebx
		call update_missiles
		popl %ebx
		movl %ebx, previous_missile
		movl NEXT(%ebx), %ebx
		jmp CHECK_PTR
	NULL:
		# PART 2
		# check each of the bottom 3 Bytes of base_alive for the status of each base and print accordingly
		movl base_alive, %ebx

		# base 1
		andl $0x00FF0000, %ebx
		shr $HIGHHALF_SHIFT, %ebx
		cmpb $0, %bl
		je DEAD_BASE1
		# PRINT BASE 1 PIC
		BASE1:
		movl $BASE1_OFFSET, %edx
		pushl %edx
		call print_base
		popl %edx
		jmp BASE2
		DEAD_BASE1:
		movl $BASE1_OFFSET, %edx
		pushl %edx
		call print_dead_base
		popl %edx
		# base 2
		movl base_alive, %ebx
		andl $0x0000FF00, %ebx
		shr $SHIFT8, %ebx
		cmpb $0, %bl
		je DEAD_BASE2
		# PRINT BASE 2 PIC
		BASE2:
		movl $BASE2_OFFSET, %edx
		pushl %edx
		call print_base
		popl %edx
		jmp BASE3
		DEAD_BASE2:
		movl $BASE2_OFFSET, %edx
		pushl %edx
		call print_dead_base
		popl %edx
		# base 3
		movl base_alive, %ebx
		andl $0x000000FF, %ebx
		cmpb $0, %bl
		je DEAD_BASE3
		BASE3:
		movl $BASE3_OFFSET, %edx
		pushl %edx
		call print_base
		popl %edx
		jmp DRAW_XHAIR
		DEAD_BASE3:
		movl $BASE3_OFFSET, %edx
		pushl %edx
		call print_dead_base
		popl %edx

		# Part 3
		# redraw crosshairs
		DRAW_XHAIR:
		movl crosshairs_x, %esi
		movl crosshairs_y, %edi
		imull $BYTESPERCHAR, %esi
		imull $BYTESPERROWCHAR, %edi
		addl %edi, %esi
		movl %esi, %eax
		movb $XHAIR, %cl
		pushl %edx
		call mp1_poke
		popl %edx


		# stack teardown
		popl %ecx
		popl %edx
		popl %eax
		popl %edi
		popl %esi
		popl %ebx
		leave
		ret


# function to print the base PIC
# prototype: print_base(base_starting_offset)
# INPUT: the starting offset into video memory of the base to be printed
# OUTPUT: NONE, the function will print the base picture
# Registers: Standard C calling convention
print_base:

		pushl %ebp
		movl %esp, %ebp
		pushl %ebx
		pushl %esi
		pushl %edi
		pushl %eax
		pushl %edx
		pushl %ecx

		movl 8(%ebp), %eax			# get the starting offset
		movb base_pic+BASECHAR_SEL0, %cl
		call mp1_poke
		addl $BYTESPERCHAR, %eax	# increment the offset
		movb base_pic+BASECHAR_SEL1, %cl
		call mp1_poke
		addl $BYTESPERCHAR, %eax	# increment the offset
		movb base_pic+BASECHAR_SEL2, %cl
		call mp1_poke
		addl $BYTESPERCHAR, %eax	# increment the offset
		movb BASECHAR_SEL3+base_pic, %cl
		call mp1_poke
		addl $BYTESPERCHAR, %eax	# increment the offset
		movb $LAST_BASE_CHAR, %cl
		call mp1_poke

		# stack teardown
		popl %ecx
		popl %edx
		popl %eax
		popl %edi
		popl %esi
		popl %ebx
		leave

		ret



# function to print the dead base
# prototype: print_dead_base(base_starting_offset)
# INPUT: the starting offset into video memory of the base to be printed
# OUTPUT: NONE, the function prints the dead base pic
# Registers: Standard C calling convention
print_dead_base:
		pushl %ebp
		movl %esp, %ebp
		pushl %ebx
		pushl %esi
		pushl %edi
		pushl %eax
		pushl %edx
		pushl %ecx

		movl 8(%ebp), %eax			# get the starting offset
		movb BASECHAR_SEL0+dead_base_pic, %cl
		call mp1_poke
		addl $BYTESPERCHAR, %eax	# increment the offset
		movb BASECHAR_SEL1+dead_base_pic, %cl
		call mp1_poke
		addl $BYTESPERCHAR, %eax	# increment the offset
		movb BASECHAR_SEL2+dead_base_pic, %cl
		call mp1_poke
		addl $BYTESPERCHAR, %eax	# increment the offset
		movb BASECHAR_SEL3+dead_base_pic, %cl
		call mp1_poke
		addl $BYTESPERCHAR, %eax	# increment the offset
		movb BASECHAR_SEL4+dead_base_pic, %cl
		call mp1_poke

		# stack teardown
		popl %ecx
		popl %edx
		popl %eax
		popl %edi
		popl %esi
		popl %ebx
		leave

		ret




# int mp1_ioctl(unsigned long arg, unsigned int cmd)
# The dispatch function for the MP1 ioctls - should use the cmd argument
# and a jumptable to execute one of the specific ioctls implemented below.
# Inputs   : unsigned long arg - parameter to the mp1_ioctl_....
#          : unsigned int cmd  - specifies which mp1_ioctl_... function
#          :                     to execute
# Outputs  : Returns an integer - depends on which ioctl() function is called
# Registers: Standard C calling convention

.globl mp1_ioctl
mp1_ioctl:
	# dispatcher function -- no stack setup and teardown
	# ESP + 4 == arg
	# ESP + 8 == cmd
	# Jump to ioctl function depending on cmd value
	movl 8(%esp), %ecx
	cmpl $ENDGAME_CMD, %ecx
	ja dispatcher_failure
	jmp *jumptable(,%ecx,4)
	ret

dispatcher_failure:		# helper function to return -1 if the dispatcher fails
	movl $-1, %eax
	ret

jumptable:
	.long mp1_ioctl_startgame, mp1_ioctl_addmissile, mp1_ioctl_movexhairs, mp1_ioctl_getstatus, mp1_ioctl_endgame

# ----------------- Functions private to this file -------------------


# function that updates a missile. There are three scenarios
# 1. The missile is off the screen. This function should erase the missile and remove it from the linked list
# 2. The missile is at the destination and not expoding. This function will call the missile_explode function and will decrement the
# EXPLODED value in the missile struct
# 3. The missile is currently exploding. This function reacts as described in #2
# 4. The missile is moving. This function should redraw the missile character
# INPUT: pointer to the missile to update
# OUTPUT: NONE, this function will redraw the missiles accordingly
# Registers: Standard C calling convention
update_missiles:
		#  save registers
		pushl %ebp
		movl %esp, %ebp
		pushl %esi
		pushl %edi
		pushl %edx
		pushl %ecx


		movl 8(%ebp), %ebx
		movl EXPLODED(%ebx), %edx
		cmpl $0, %edx
		jne EXPLODING
		# ERASE
		movb $SPACE, %cl
		movl X(%ebx), %esi
		movl Y(%ebx), %edi
		shr $HIGHHALF_SHIFT, %esi
		shr $HIGHHALF_SHIFT, %edi
		imull $BYTESPERROWCHAR,%edi             # edi <--- edi*160
		imull $BYTESPERCHAR,%esi,%eax          # eax <--- esi*2
		addl %edi, %eax             # eax <--- eax+edi
		pushl %edx
		call mp1_poke
		popl %edx

		# UPDATE POSITION
		movl X(%ebx), %esi
		movl Y(%ebx), %edi
		addl VX(%ebx), %esi
		addl VY(%ebx), %edi
		movl %esi, X(%ebx)
		movl %edi, Y(%ebx)      # update x and y

		shr $HIGHHALF_SHIFT, %esi           # get coordinates
		shr $HIGHHALF_SHIFT, %edi           # get coordinates

		# BOUNDS CHECK
		cmpl $XMAX, %esi
		jg REMOVE_MISSILE
		cmpl $0, %esi
		jle REMOVE_MISSILE
		cmpl $YMAX, %edi
		jg REMOVE_MISSILE
		cmpl $0, %edi
		jle REMOVE_MISSILE

		# DESTINATION CHECK
		cmpl DEST_X(%ebx), %esi
		jne REDRAW_MISSILE
		cmpl DEST_Y(%ebx), %edi
		jne REDRAW_MISSILE

	EXPLODING:
		pushl %ebx
		pushl %edx
		pushl %edi
		pushl %esi
		pushl %eax
		pushl %ecx

		pushl %ebx
		call missile_explode
		popl %ebx

		popl %ecx
		movl %eax, %ecx
		popl %eax
		popl %esi
		popl %edi
		popl %edx
		popl %ebx

		add $-1, EXPLODED(%ebx)
		cmpl $0, %ecx           # check return of missile_explode
		je CHECK_EXPLODED

		pushl %ebx
		pushl %edx
		pushl %edi
		pushl %esi
		pushl %eax
		pushl %ecx
		call mp1_notify_user
		popl %ecx
		movl %eax, %ecx
		popl %eax
		popl %esi
		popl %edi
		popl %edx
		popl %ebx

	CHECK_EXPLODED:

		cmpl $0, EXPLODED(%ebx)
		jne REDRAW_EXPLOSION
		movl $0, VX(%ebx)
		movl $0, VY(%ebx)
		jmp REMOVE_MISSILE
	REDRAW_MISSILE:

		movl X(%ebx), %esi
		movl Y(%ebx), %edi
		shr $HIGHHALF_SHIFT, %esi
		shr $HIGHHALF_SHIFT, %edi

		imull $BYTESPERROWCHAR, %edi
		imull $BYTESPERCHAR,%esi,%eax
		addl %edi, %eax
		movb C(%ebx), %cl
		call mp1_poke
		jmp UPDATE_DONE

	REDRAW_EXPLOSION:

		movl X(%ebx), %esi
		movl Y(%ebx), %edi
		shr $HIGHHALF_SHIFT, %esi
		shr $HIGHHALF_SHIFT, %edi
		imull $BYTESPERROWCHAR, %edi
		imull $BYTESPERCHAR,%esi,%eax
		addl %edi, %eax
		movb $EXPLOSION, %cl
		call mp1_poke
		jmp UPDATE_DONE

	REMOVE_MISSILE:
/*		pushl %edx
		pushl %eax
		cmpl mp1_missile_list, %ebx
		jne NOT_HEAD
		cmpl $0x0, %ebx
		je UPDATE_DONE
		movl NEXT(%ebx), %edx
		movl %edx, mp1_missile_list
		movl $0x0, NEXT(%ebx)
		jmp FREE
	NOT_HEAD:
		movl mp1_missile_list, %edx

	LIST_LOOP:
		cmpl NEXT(%edx), %ebx
		jne ADVANCE

		movl NEXT(%ebx), %eax
		movl %edi, NEXT(%edx)
		movl $0x0, NEXT(%ebx)
		jmp FREE
	ADVANCE:
		movl NEXT(%edx), %edx
		jmp LIST_LOOP
	GET_NEXT:
		movl NEXT(%ebx), %ebx
	FREE:
		movl NEXT(%ebx), %edx
		call mp1_free
		movl %edx, %ebx
		popl %eax
		popl %edx */
	UPDATE_DONE:
		popl %ecx
		popl %edx
		popl %edi
		popl %esi
		leave
		ret

# This function initializes all variables
# INPUTS: NONE
# OUTPUT: returns 0 always
# Registers: Standard C calling convention
mp1_ioctl_startgame:
		# stack setup
		pushl %ebp
		movl %esp, %ebp
		pushl %ebx
		pushl %esi
		pushl %edi

		movl $CENTER_X, crosshairs_x
		movl $CENTER_Y, crosshairs_y
		movl $0x0, mp1_score
		movl $0x0, mp1_missile_list
		movl $0x00FFFFFF, base_alive

		# stack teardown
		popl %edi
		popl %esi
		popl %ebx
		movl $0, %eax
		leave
		ret
# This function adds a missile struct to a linked list by coping the struct from user space into kernel space and then performing a linked
# list add operation
# INPUTS: struct missile* user missile
# OUTPUT: returns 0 for success and -1 for failure
# Registers: Standard C calling convention
mp1_ioctl_addmissile:
		#  save registers
		pushl %ebp
		movl %esp, %ebp
		pushl %ebx
		pushl %esi
		pushl %edi
		pushl %edx
		pushl %ecx
		# get missle pointer in user space from stack: EBP + 8
		movl 8(%ebp), %esi
		movl $MISSILE_SIZE, %edi
		# allocate memory for the missile in kernel space
		pushl %esi
		pushl %ecx
		pushl %edi
		call mp1_malloc
		popl %edi
		popl %ecx
		popl %esi
		cmpl $0, %eax
		je ERROR_ADDMISSILE
		movl %eax, %edx			# move pointer to edx
		# copy from user space
		pushl %ecx
		pushl %edi			# unsigned long size n
		pushl %esi			# *from
		pushl %edx 			# *to
		call mp1_copy_from_user
		popl %edx
		popl %esi
		popl %edi
		popl %ecx

		cmpl $0, %eax
		jne ERROR_ADDMISSILE
		# set next of struct to current head
		movl mp1_missile_list, %edi
		movl %edi, NEXT(%edx)
		# set current head to address of this struct
		movl %edx, mp1_missile_list
		# set EAX to 0 to indicate
		movl $0, %eax
		jmp DONE_ADDMISSILE
		ERROR_ADDMISSILE:
		pushl %eax
		call mp1_free
		movl $-1, %eax
		DONE_ADDMISSILE:
		popl %ecx
		popl %edx
		popl %edi
		popl %esi
		popl %ebx
		leave
		ret

# This function takes a delta packed value where the top half represents deltaY and the bottom half represents deltaX.
# this function adds the changes and redraws the crosshairs
# INPUTS: unsigned long xhair delta packed
# OUTPUT: will always return
# Registers: Standard C calling convention

mp1_ioctl_movexhairs:
		# stack SETUP
		pushl %ebp
		movl %esp, %ebp
		pushl %ebx
		pushl %esi
		pushl %edi
		pushl %edx
		pushl %ecx

		movl crosshairs_x, %esi
		movl crosshairs_y, %edi
	ERASE_XHAIR:
		movb $SPACE, %cl
		imull $BYTESPERCHAR, %esi
		imull $BYTESPERROWCHAR, %edi
		addl %edi, %esi			# store the offset in eax
		movl %esi, %eax
		pushl %edx
		call mp1_poke
		popl %edx


		movl crosshairs_x, %esi
		movl crosshairs_y, %edi
		movl 8(%ebp), %ebx			# get the arguement
		movl %ebx, %ecx				# make a copy to bitmask
		andl $BITMASK, %ecx			# isolate the lower word of the arguement
		pushl %eax
		movl %ecx, %eax
		andl $0x00008000, %eax
		cmpl $0, %eax
		je SIGNEDX
		orl $0xFFFF0000, %ecx

	SIGNEDX:
		popl %eax
		cmpl $0, %ecx
		je CHECK_Y
		addl %ecx, %esi
		cmpl $0, %esi
		jge NOTMINEDGE_X
		movl $0, %esi
		jmp CHECK_Y
	NOTMINEDGE_X:					# now check the max edge
		cmpl $XMAX, %esi
		jl CHECK_Y
		movl $XMAX-1, %esi

	CHECK_Y:
		movl %esi, crosshairs_x
		movl crosshairs_y, %edi
		shrl $HIGHHALF_SHIFT, %ebx			# right shift the arg
		movl %ebx, %ecx						# make a copy
		andl $BITMASK, %ecx					# bitmask the lower word
		pushl %eax
		movl %ecx, %eax
		andl $0x00008000, %eax
		cmpl $0, %eax
		je SIGNEDY
		orl $0xFFFF0000, %ecx
	SIGNEDY:
		cmpl $0, %ecx
		je CALC_OFFSET
		addl %ecx, %edi
		cmpl $0, %edi
		jge NOTMINEDGE_Y
		movl $0, %edi
	NOTMINEDGE_Y:
		cmpl $YMAX, %edi
		jl CALC_OFFSET							# if y is on the screen, go to calculalte the offset
		movl $YMAX-2, %edi
	CALC_OFFSET:
		# calculate offset here
		# reload coordinate values
		movl %edi, crosshairs_y
		movl crosshairs_x, %esi

		imull $BYTESPERCHAR, %esi
		imull $BYTESPERROWCHAR, %edi
		addl %edi, %esi
		movl %esi, %eax						# store the offset in eax

	DRAW:
		movb $XHAIR, %cl
		pushl %edx
		call mp1_poke
		popl %edx

		popl %ecx
		popl %edx
		popl %edi
		popl %esi
		popl %ebx
		movl $0, %eax
		leave
		ret

# This function gets the status of the game and copies it to user space
# A 32-bit integer is copied into user space with the low 16 bits holding the score and bits 16, 17, and 18 holding the status of the bases
# INPUTS: unsigned long* ptr: a pointer to the copy location in user space
# OUTPUTS: signed long
# REGISTERS: Standard C calling convention
mp1_ioctl_getstatus:
		# stack setup
		pushl %ebp
		movl %esp, %ebp
		pushl %ebx
		pushl %esi
		pushl %edi
		pushl %edx
		pushl %ecx

		movl 8(%ebp), %esi
		# allocate 32-bit (4 B) in kernel space
		movl $UNSIGNED_LONG_PTR_SIZE, %edx
		pushl %eax
		pushl %ecx
		pushl %edx
		call mp1_malloc
		popl %edx
		popl %ecx
		movl %eax, %edx			# no longer need the size of the pointer, can reuse edx now
		popl %eax

		# copy score into 15:0 of allocated memory
		movl mp1_score, %edi
		movl %edi, (%edx)
		andl $BITMASK, (%edx)
		movl base_alive, %ebx
		# copy base status into 16,17,18
	BASE1_GETSTATUS:
		movb BASE1_SELECT(%ebx), %cl
		cmpb $0, %cl
		je BASE2_GETSTATUS
		addl $BASE1_ALIVE, (%edx)
	BASE2_GETSTATUS:
		movb BASE2_SELECT(%ebx), %cl
		cmpb $0, %cl
		je BASE3_GETSTATUS
		addl $BASE2_ALIVE, (%edx)
	BASE3_GETSTATUS:
		movb BASE3_SELECT(%ebx), %cl
		cmpb $0, %cl
		je COPYTOUSER
		addl $BASE3_ALIVE, (%edx)

	COPYTOUSER:
		# copy to user SPACE
		movl $UNSIGNED_LONG_PTR_SIZE, %edi
		pushl %edi
		pushl %edx
		pushl %esi
		call mp1_copy_to_user
		popl %esi
		popl %edx
		popl %edi

		cmpl $0, %eax
		jne ERROR_GETSTATUS
		jmp TEARDOWN
	ERROR_GETSTATUS:
		movl $-1, %eax
		# teardow
	TEARDOWN:
		popl %ecx
		popl %edx
		popl %edi
		popl %esi
		popl %ebx
		leave
		ret

# This function cleans up the linked list data structure by freeing any remaining missiles
# INPUTS: NONE
# OUTPUTS: NONE
# REGISTERS: Standard C calling convention
mp1_ioctl_endgame:
		pushl %ebp
		movl %esp, %ebp
		pushl %ebx
		pushl %edx

		movl mp1_missile_list, %ebx
		cmpl $0x0, %ebx
		je END_DONE

	ENDLOOP:
		cmpl $0, %ebx
		je END_DONE
		movl NEXT(%ebx), %edx
		pushl %ebx
		pushl %edx
		call mp1_free
		popl %edx
		popl %ebx
		movl %edx, %ebx
		cmpl $0x0, %ebx
		jne ENDLOOP
	END_DONE:
		popl %edx
		popl %ebx
		movl $0, %eax
		leave
		ret
