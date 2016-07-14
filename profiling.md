# Profiling Note

## Floating Point calculator (FPU)

- Ticks frequency: 84000000 (half of main frequency)
- Ticks prescaler: 83
- Ticks period: 2
- Result: 1 tick = *2us*

- Optimization level: *O1*
- Not optimized for time

Using the following code:

	s32 starting_ticks = get_full_ticks();
	
	volatile float result[200] = {0.0f};
	for (u16 i=0;i<200;i++){
		result[i] = (float)rand() / (float)RAND_MAX;
	}
	
	for (u16 i=1;i<199;i++){
		result[i] = ((result[i]*65.37f*result[i-1] - result[i-2])+14.589f)/562.3458f;
	}

	for (u16 i=198;i>1;i--){
		result[i] = result[i]/result[i+1]/result[i-1];
	}
	
	s32 end_ticks = get_full_ticks();

Then find end_ticks - starting_ticks.
- With single point precision *disabled*, ticks diff = *404000*
- With it *enabled*, ticks diff = *50001*

Conclusion: FPU helps.

***

## Comparing sine/cosine funciton in different libraries (with FPU)
