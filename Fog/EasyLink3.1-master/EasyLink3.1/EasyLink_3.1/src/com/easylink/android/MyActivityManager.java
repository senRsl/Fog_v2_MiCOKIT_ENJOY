package com.easylink.android;

import java.util.LinkedList;
import java.util.List;

import android.app.Activity;

public class MyActivityManager {

	private List<Activity> activityList = new LinkedList<Activity>();
	private static MyActivityManager instance;

	private MyActivityManager() {
	}

	public static MyActivityManager getInstance() {
		if (null == instance) {
			instance = new MyActivityManager();
		}
		return instance;
	}

	public void addActivity(Activity activity) {
		activityList.add(activity);
	}

	public void subExit() {
		for (Activity activity : activityList) {
			activity.finish();
		}
		activityList.clear();
		System.gc();
	}

	public void exit() {
		for (Activity activity : activityList) {
			activity.finish();
		}
		android.os.Process.killProcess(android.os.Process.myPid());
		System.gc();
		System.exit(0);
	}
}
