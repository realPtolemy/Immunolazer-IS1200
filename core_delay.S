/* SOFTWARE DELAY USED BY MAIN FUNCTION */
/* The code below is written by: Love Mitteregger & Peter Byström */

	.global	delay
	.global	enable_interrupt

delay:
	move	$s0,$a0			# Store retreived argument $a0 value at #s0
delaywhile:
	beqz	$s0,delaydone	# If value in $s0 is zero, jump to delay done (end while loop)
	nop
	sub		$s0,$s0,1		# During each itteration, decrement while loop pointer in $s0 with 1
	move	$s1,$0			# Reset for loop 'i-pointer'
delayfor:
	beq		$s1,3772,delaywhile
	nop
	addi	$s1,$s1,1		# Increment loop 'i-pointer' with 1 during each loop
	j		delayfor
	nop
	j		delaywhile
	nop
delaydone:
	move	$a0,$s0			# Return $a0 value stored in $s2
	move	$s0,$0			# Reset $s0, in accordance with assembly conventions
	move	$s1,$0			# Reset $s1
	move	$s2,$0			# Reset $s2
	jr 		$ra				# return back to main
	nop

enable_interrupt:
	ei
	jr		$ra
	nop
 	
 	

