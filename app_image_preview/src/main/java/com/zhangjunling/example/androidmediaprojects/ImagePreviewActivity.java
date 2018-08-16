package com.zhangjunling.example.androidmediaprojects;

import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.zhangjunling.example.androidmediaprojects.view.ImageSurfaceView;

import java.io.File;

public class ImagePreviewActivity extends AppCompatActivity {
    private ImageSurfaceView imageSurfaceView;
    private Button showImageView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_image);
        imageSurfaceView = this.findViewById(R.id.imageSurfaceView);
        showImageView = this.findViewById(R.id.bt_show_image);

        showImageView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String imagePath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/" + "a.png";
                File imageFile = new File(imagePath);
                if(imageFile.exists()){
                    imageSurfaceView.showBitmap2(imagePath);
                }else{
                    Log.e("zhangjunling", "imageFile not exists.");
                }

            }
        });

    }
}
