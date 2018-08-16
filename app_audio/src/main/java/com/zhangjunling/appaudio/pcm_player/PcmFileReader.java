package com.zhangjunling.appaudio.pcm_player;

import android.util.Log;

import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;


/**
 * 获取PCM数据
 */
public class PcmFileReader {
    private DataInputStream mDataInputStream;

    public void openFile(String filePath) throws IOException{
        if(null != mDataInputStream){
            closeFile();
        }

        File file = new File(filePath);
        if(file == null || !file.exists() ){
            throw new IllegalArgumentException("file 文件为空或者不存在.");
        }

        mDataInputStream = new DataInputStream(new FileInputStream(filePath));

    }

    /**
     * 在线程中执行读操作
     * @param buffer
     * @param offset
     * @param count
     * @return
     */
    public int readData(byte[] buffer, int offset, int count) {
        if (mDataInputStream == null) {
            return -1;
        }

        try {
            int nbytes = mDataInputStream.read(buffer, offset, count);
            if (nbytes == -1) {
                return 0;
            }
            return nbytes;
        } catch (IOException e) {
            e.printStackTrace();
        }

        return -1;
    }


    public void closeFile() throws IOException {
        if (mDataInputStream != null) {
            mDataInputStream.close();
            mDataInputStream = null;
        }
    }
}
