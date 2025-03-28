轨迹偏移库，提供armv7a、arm64-v8a、x86、x86-64四个版本的静态库与动态库

如果使用静态库，直接调用wgs2gcj.h中的接口即可

如果使用动态库，在java中的使用方式如下：
public class Jni_wgs2gcj {
	public static native int wgs2gcj(double[] wgs, double[] gcj, int hei);

	public static native int wgs2gcj_second(double[] wgs, double[] gcj, int hei);

	static {
		System.loadLibrary("Jni_wgs2gcj");
	}
}