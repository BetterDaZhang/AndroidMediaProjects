package com.zhangjunling.example.androidmediaprojects.view;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.os.Environment;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class ImageSurfaceView extends SurfaceView implements SurfaceHolder.Callback{
    private SurfaceHolder surfaceHolder;
    private int width;
    private int height;

    public ImageSurfaceView(Context context) {
        this(context,null);
    }

    public ImageSurfaceView(Context context, AttributeSet attrs) {
        this(context, attrs,0);
    }

    public ImageSurfaceView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    private void init() {
        getHolder().addCallback(this);
    }


    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        surfaceHolder = holder;
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        this.width = width;
        this.height = height;

        String imagePath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/" + "a.png";
        showBitmap(imagePath);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    public void showBitmap(String imagePath){
        Bitmap bitmap = BitmapFactory.decodeFile(imagePath);
        if(bitmap == null){
            throw new IllegalArgumentException("bitmap is null.");
        }
        Canvas canvas = surfaceHolder.lockCanvas();

        //在Canvas上面绘制图片
        canvas.drawBitmap(bitmap,0,0,new Paint());
        surfaceHolder.unlockCanvasAndPost(canvas);

        if(null != bitmap){
            bitmap.recycle();
        }
    }

    public void showBitmap2(String imagePath){
        Bitmap bitmap = BitmapFactory.decodeFile(imagePath);
        if(bitmap == null){
            throw new IllegalArgumentException("bitmap is null.");
        }
        Canvas canvas = surfaceHolder.lockCanvas();

        //在Canvas上面绘制图片
//        canvas.drawBitmap(bitmap,0,0,new Paint());
        Rect src = new Rect();
        src.left = width/2;
        src.top = height/2;
        src.right = width;
        src.bottom = height;

        Rect dst = new Rect();
        dst.left = 0;
        dst.top = 0;
        dst.right = width;
        dst.bottom = height;
        canvas.drawBitmap(bitmap, src, dst, new Paint());
        surfaceHolder.unlockCanvasAndPost(canvas);

        if(null != bitmap){
            bitmap.recycle();
        }
    }
}
