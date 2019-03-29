package ua.secure;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import java.io.File;

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
            File dbFolder = new File(getFilesDir(), "databases");
            if(!dbFolder.exists()) {
                dbFolder.mkdirs();
            }
            File dbFile = new File(dbFolder, "secureDB.db");
            SQLiteDatabase database = new SQLiteDatabase(dbFile.getAbsolutePath());
            database.execute("CREATE TABLE IF NOT EXISTS DATA(id INTEGER PRIMARY KEY, name TEXT, value TEXT);");
            database.execute("INSERT INTO DATA(id, name, value) VALUES(1, 'fruit', 'mango');");
            database.execute("INSERT INTO DATA(id, name, value) VALUES(2, 'фрукт', 'яблуко');");

            SQLiteCursor cursor = database.query("SELECT * FROM DATA;");
            if(cursor != null && cursor.next()) {
                String data = cursor.stringValue("name") + ":" + cursor.stringValue("value");
                tv.setText(data);
            }
            cursor.dispose();
            database.close();
        } catch (Exception ex) {
            tv.setText(ex.toString());
            Log.e("SecureNative", "Error:" + ex.toString());
        }

    }
}
