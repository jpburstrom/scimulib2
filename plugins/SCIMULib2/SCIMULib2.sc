SCIMULib2 : MultiOutUGen {
	*kr { |trig|
		/* TODO */ ^this.multiNew('control', trig);
	}

	init { arg ... theInputs;
		inputs = theInputs;
		^this.initOutputs(3, rate);
	}
}
