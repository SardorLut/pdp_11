000000:		    
000000:		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
000000:		; add [r0,r1] with [r2,r3]. Store result in [r4,r5]
000000:		; [1, 100001] + [2, 100050] = [4, 00051]	
000000:		; expected: r4=4 r5=51
000000:		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
000000:		
000000:			.=1000
001000:			mov #1, r0
	 012700
	 000001
001004:			mov #100001, r1
	 012701
	 100001
001010:			mov #2, r2
	 012702
	 000002
001014:			mov #100050, r3
	 012703
	 100050
001020:			
001020:			mov r2, r4
	 010204
001022:			mov r3, r5
	 010305
001024:			add r1, r5
	 060105
001026:			adc r4
	 005504
001030:			add r0, r4
	 060004
001032:			halt
	 000000