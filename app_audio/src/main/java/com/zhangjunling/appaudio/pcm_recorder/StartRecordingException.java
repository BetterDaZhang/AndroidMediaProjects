package com.zhangjunling.appaudio.pcm_recorder;

public class StartRecordingException extends Exception {

	private static final long serialVersionUID = -6097897894257568186L;

	public StartRecordingException() {
		super("开启录音器失败");
	}
}
