package ua.secure;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import ua.secure.sqlite.SQLiteCursor;
import ua.secure.sqlite.SQLiteDatabase;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("android_sqlite");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);

        try {
            SQLiteDatabase database = new SQLiteDatabase("secureDB");
            database.execute("CREATE TABLE IF NOT EXIST DATA(id INTEGER PRIMARY KEY, name TEXT, value TEXT);");
            database.execute("INSERT INTO DATA VALUES(1, fruit, mango);");
            database.execute("INSERT INTO DATA VALUES(2, фрукт, яблуко);");

            SQLiteCursor cursor = database.query("SELECT * FROM DATA;");
            if(cursor != null && cursor.next()) {
                String data = cursor.stringValue("name") + ":" + cursor.stringValue("value");
                tv.setText(data);
            }
            cursor.dispose();
            database.close();
        } catch (Exception ex) {
            Log.e("SecureNative", "Error:" + ex.toString());
        }

    }
}
