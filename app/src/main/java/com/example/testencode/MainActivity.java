package com.example.testencode;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;

import com.example.administrator.hellojni_ndkbuild.R;
import com.nd.hilauncherdev.theme.parse.EncodeTools;
import com.nd.hilauncherdev.theme.parse.EncodeTools.ITEM;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class MainActivity extends Activity implements OnClickListener {
	Button button;
	ImageView view;
	ITEM items[];
	//String fileString="/sdcard/storage/sdcard0/PandaHome2/Themes/Hello_V6_21381935102742/icons/drawable-1280x7200/collectall.dtc";
	String fileString="/sdcard/collectall.dtc";
//	String fileString="/storage/emulated/0/collectall.dtc";
//	String fileString="/sdcard/deviceid.txt";
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main1);
		button=(Button)findViewById(R.id.button1);
		button.setOnClickListener(this);
		view=(ImageView)findViewById(R.id.imageView1);
	}

	int count=0;
	int i=0;
	@Override
	public void onClick(View v) {
		
		getSaveAllPic();
//		new Thread()
//		{
//			@Override
//			public void run() {
//				EncodeTools.getItemInfo(fileString);
//				//super.run();
//			}
//		}.start();
		
//		boolean result1=EncodeTools.WriteImei(fileString, "11111111");
//		if(result1)
//		{
//			Log.e("zhou", "add succeed");
//		}else {
//			Log.e("zhou", "add fasle");
//		}
//		
		
//		EncodeTools.getItemInfo(fileString);
//		byte result[]=EncodeTools.getResource(fileString,"panda_icon_background_mask");
//		
//		
//		if(result==null)
//		{
//			return;
//		}
//	
//		InputStream is=new ByteArrayInputStream(result);
//		Bitmap bitmap=BitmapFactory.decodeStream(is);
//		BitmapDrawable bd=new BitmapDrawable(bitmap);
//		view.setImageBitmap(bitmap);
//		view.invalidate();
	}
	
	
	public void getSaveAllPic() {
		ITEM item[] = EncodeTools.getItemInfo(fileString);
		for (int i = 0; i < item.length; i++) {
			ITEM info=item[i];
			
			
			byte result[]=EncodeTools.getResource(fileString,info.startPost,info.len);
			if(result==null)
			{
				continue;
			}
			InputStream is=new ByteArrayInputStream(result);
			Bitmap bitmap=BitmapFactory.decodeStream(is);
			saveBitmap(bitmap,"p"+i);
		}

	}
	
	
	/** 保存方法 ***/
	public static void saveBitmap(Bitmap bm, String picName) {
		// Log.e("zhou", "保存图片");
		File f = new File("/mnt/sdcard/zheng/", picName + ".png");

		try {
			if (f.exists()) {
				f.delete();
			} else {
				f.createNewFile();
			}
			FileOutputStream out = new FileOutputStream(f);
			bm.compress(Bitmap.CompressFormat.PNG, 90, out);
			out.flush();
			out.close();

		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
