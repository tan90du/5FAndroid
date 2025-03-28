# Add project specific ProGuard rules here.
# By default, the flags in this file are appended to flags specified
# in C:\Users\xiaopo.hxp\AppData\Local\Android\sdk/tools/proguard/proguard-android.txt
# You can edit the include path and order by changing the proguardFiles
# directive in build.gradle.
#
# For more details, see
#   http://developer.android.com/guide/developing/tools/proguard.html

# Add any project specific keep options here:

# If your project uses WebView with JS, uncomment the following
# and specify the fully qualified class name to the JavaScript interface
# class:
#-keepclassmembers class fqcn.of.javascript.interface.for.webview {
#   public *;
#}

-dontpreverify
-flattenpackagehierarchy
-optimizationpasses 5
-dontusemixedcaseclassnames
-dontskipnonpubliclibraryclasses
-verbose

-keep public class * extends android.app.Activity
-keep public class * extends android.app.Application
-keep public class * extends android.app.Service
-keep public class * extends android.content.BroadcastReceiver
-keep public class * extends android.content.ContentProvider
-keep public class * extends android.app.backup.BackupAgentHelper
-keep public class * extends android.preference.Preference
-keep public class com.autonavi.common.tool.dumpcrash { *; }
-keep class com.uc.crashsdk.** { *; }
-keep interface com.uc.crashsdk.** { *; }
-keep public class * extends android.app.Fragment
-keep public class * extends android.support.v4.app.Fragment

-keepclasseswithmembers class * {
    public <init>(android.content.Context, android.util.AttributeSet);
}

-keepclasseswithmembers class * {
    public <init>(android.content.Context, android.util.AttributeSet, int);
}

-keepclassmembers class * extends android.app.Activity {
   public void *(android.view.View);
}

-keepclassmembers enum * {
    public static **[] values();
    public static ** valueOf(java.lang.String);
}

# As described in tools/proguard/examples/android.pro - ignore all warnings.
-dontwarn android.support.v4.**

# For native methods, ָ�����������е�JNI����
-keepclasseswithmembernames class * {
    native <methods>;
}

# The maps library uses custom Parcelables.  Use this rule (which is slightly
# broader than the standard recommended one) to avoid obfuscating them.
-keepclassmembers class * implements android.os.Parcelable {
    static *** CREATOR;
}

# The maps library uses serialization so keep this.
-keepclassmembers class * implements java.io.Serializable {
    static final long serialVersionUID;
    static final java.io.ObjectStreamField[] serialPersistentFields;
    private void writeObject(java.io.ObjectOutputStream);
    private void readObject(java.io.ObjectInputStream);
    java.lang.Object writeReplace();
    java.lang.Object readResolve();
}

-keep public class com.autonavi.plugin.impl.SmartEyeActivity { *; }
-keep public class com.autonavi.plugin.impl.SmartEyeApplication { *; }
-keep public class com.autonavi.plugin.impl.SmartEyeReceiver { *; }
-keep public class com.autonavi.plugin.impl.SmartEyeService { *; }
-keep public class com.autonavi.** { *; }

-dontwarn org.codehaus.mojo.**
-keep public class org.codehaus.mojo.**

-dontwarn java.nio.**
-keep public class java.nio.**
