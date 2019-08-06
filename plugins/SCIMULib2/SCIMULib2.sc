SCIMULib2 : MultiOutUGen {
	*kr { |input, gain|
		/* TODO */ ^this.multiNew('control', input, gain);
	}

	init { arg ... theInputs;
		inputs = theInputs;
		^this.initOutputs(3, rate);
	}
}
