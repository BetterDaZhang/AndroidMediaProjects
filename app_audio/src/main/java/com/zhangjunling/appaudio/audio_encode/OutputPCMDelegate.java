package com.zhangjunling.appaudio.audio_encode;

public interface OutputPCMDelegate {
    public void outputPCMPacket(byte[] data);
}
