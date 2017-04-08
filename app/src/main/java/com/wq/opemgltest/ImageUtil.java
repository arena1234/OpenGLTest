package com.wq.opemgltest;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

/**
 * Created by arena on 2017/2/14.
 */

public class ImageUtil {
    public static Bitmap[] bitmap = new Bitmap[8];
    private static final int Index = 2;
    public static void init(Context context){
//        bitmap[0] = BitmapFactory.decodeResource(context.getResources(), R.drawable.img_8);
        bitmap[0] = BitmapFactory.decodeResource(context.getResources(), R.drawable.img_7);
        bitmap[1] = BitmapFactory.decodeResource(context.getResources(), R.drawable.img_6);
        bitmap[Index] = BitmapFactory.decodeResource(context.getResources(), R.drawable.img_1);
        bitmap[Index + 1] = BitmapFactory.decodeResource(context.getResources(), R.drawable.img_2);
        bitmap[Index + 2] = BitmapFactory.decodeResource(context.getResources(), R.drawable.img_3);
        bitmap[Index + 3] = BitmapFactory.decodeResource(context.getResources(), R.drawable.img_4);
        bitmap[Index + 4] = BitmapFactory.decodeResource(context.getResources(), R.drawable.img_5);
    }
}
