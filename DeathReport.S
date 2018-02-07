
! Number of windows in MDPA = 8
#define  NWINDOWS  8


! //! Assembler file - MDPA-Leon2 - JGomez - 26/10/2017
    .file     "DeathReport.s"
    .section  ".text"
    .align    4
	.global   asw_create_dr_regs
    .type     asw_create_dr_regs, #function

    .proc     04


! //-----------------------------------------------------------------------------------------------------------------
! // int asw_create_dr_regs(int userBuffer)
! //-----------------------------------------------------------------------------------------------------------------
! //! \brief     This function is used to copy all the registers inside the register window scheme into a user 
! //!             memory, in order to create the death report.
! //!            It will move through the register windows copying in consecutive order all the %i and %l registers. 
! //!            Global %g registers will be used for loop management, so they will be copied first into the user buffer. 
! //!            The main registers (PSR, WIM, TBR, Y) are also copied at the end of the table.
! //!            During the function execution, WIM will be set to "all windows are valid", and therefore this code must be 
! //!             executed with S=1 and ET=0. Configuration S=1 is assumed by this function, not enforced. Therefore the user
! //!             must handle that configuration. However ET=0 will be internally forced (regardless if it is already true).
! //!            For instance, if the caller function cannot guarantee that, then it can make  a specific SW trap call (e.g. ta 0x33),
! //!             and have the trap handler simply call this function.
! //!             
! //! \param[in] userBuffer: The area of memory where the register contents will be placed. It must be aligned to 4 bytes.
! //!
! //! \return Value of the user buffer running pointer at the end of the process, i.e sizeOfReport = return - inputParam
! //!
! //! \pre  Some registers are already initialized: \n
! //!          ->  %o0 = userBuffer parameter value
! //!          ->  %o7 = return address to caller
! //!             
! //! \Note This is a pseudo-leaf function, but not in the usual full meaning. Restore instruction will be used, but the caller 
! //!        may assume that its %i and %l registers are not changed (as a leaf function would). Return address is in %o7.
! //!             
! //! \Note This code must be executed in superuser mode
! //!             
! //! \Note All operations are done in 32-bit mode. Therefore the user buffer must be 4-byte aligned. No internal check is performed.
! //!             
! //! \Note As most internal operations are done with ET=0, if an additional sync trap occurs (e.g. an error trap), CPU will enter error mode
! //!             
asw_create_dr_regs:

	! Copy first the %g registers. Note that %g0 is always 0, but it is added nevertheless to maintain some order
    st  %g0, [%o0]
	inc 4, %o0
    st  %g1, [%o0]
	inc 4, %o0
    st  %g2, [%o0]
	inc 4, %o0
    st  %g3, [%o0]
	inc 4, %o0
    st  %g4, [%o0]
	inc 4, %o0
    st  %g5, [%o0]
	inc 4, %o0
    st  %g6, [%o0]
	inc 4, %o0
    st  %g7, [%o0]
	inc 4, %o0

	! Now %g registers are available for internal operations

	! Store the current PSR (%g1), and set ET=0
    mov  %psr, %g1

	mov  %psr, %g3
	andn %g3, 0x20, %g3            ! negative mask the ET bit, i.e. clear ET, ET=0
    mov  %g3, %psr                 ! update the PSR
	nop; nop; nop;


	! Store the current WIM (%g2), and set to all valid so that we can jump backward and forward without traps
    mov  %wim, %g2
    mov  %g0, %wim
	nop; nop; nop;

	! Save the user parameter address in a %g register, because %o0 is going to change meaning inside the loop (after the restore)
    mov  %o0, %g3


	! repeat the loop NWINDOWS (8) times to come back to the same window
    mov  NWINDOWS, %g4

loop_store_next_window:

	! The order is local(n) + input(n) + local(n+1) + input(n+1)
    st  %l0, [%g3]
	inc 4, %g3
    st  %l1, [%g3]
	inc 4, %g3
    st  %l2, [%g3]
	inc 4, %g3
    st  %l3, [%g3]
	inc 4, %g3
    st  %l4, [%g3]
	inc 4, %g3
    st  %l5, [%g3]
	inc 4, %g3
    st  %l6, [%g3]
	inc 4, %g3
    st  %l7, [%g3]
	inc 4, %g3

    st  %i0, [%g3]
	inc 4, %g3
    st  %i1, [%g3]
	inc 4, %g3
    st  %i2, [%g3]
	inc 4, %g3
    st  %i3, [%g3]
	inc 4, %g3
    st  %i4, [%g3]
	inc 4, %g3
    st  %i5, [%g3]
	inc 4, %g3
    st  %i6, [%g3]
	inc 4, %g3
    st  %i7, [%g3]
	inc 4, %g3


	restore                         ! shift window
	deccc %g4                       ! decrement in one and set cc flags
	bne loop_store_next_window      ! jump back until counter=0
	nop


	! Add the main unwindowed registers (PSR, WIM, TBR, Y).
	!  Note that PSR and WIM are not yet restored. Thus, its %g are used instead. It is better to make this copy before traps are enabled
	!  so that all data is coherent
	st  %g1, [%g3]     ! PSR
	inc 4, %g3

	st  %g2, [%g3]     ! WIM
	inc 4, %g3

    mov %tbr, %g5
	st  %g5, [%g3]
	inc 4, %g3

    mov %y, %g5
	st  %g5, [%g3]
	inc 4, %g3


	! Restore the original WIM and PSR
    mov  %g2, %wim
	nop; nop; nop;

    mov  %g1, %psr
	nop; nop; nop;



	! Set as return the last address of the user buffer, so that the amount of data can be calcualted
    mov  %g3, %o0


    retl                            ! return from leaf function
    nop

! End of asw_create_dr_regs


