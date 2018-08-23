package com.zhangjunling.appaudio.audio_encode.android;

public interface OutputPCMDelegate {
    public void outputPCMPacketInByte(byte[] data);
    public void outputPCMPacketInShort(short[] data);
}
