SCIMULib2 : MultiOutUGen {
	*kr { |trig=1.0|
		/* TODO */ ^this.multiNew('control', trig);
	}

	init { arg ... theInputs;
		inputs = theInputs;
		^this.initOutputs(3, rate);
	}
}
