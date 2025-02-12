ExampleMixer : UGen {
    // Derive from UGen class as this is a UGen,
    // see https://docs.supercollider.online/Classes/UGen.html

    *ar {|sigA, sigB|
        // see https://docs.supercollider.online/Classes/UGen.html#*multiNew
        ^this.multiNew('audio', sigA, sigB);
    }

    *kr {|sigA, sigB|
        ^this.multiNew('control', sigA, sigB);
    }
}
