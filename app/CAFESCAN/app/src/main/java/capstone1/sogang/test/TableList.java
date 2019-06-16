package capstone1.sogang.test;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.android.volley.AuthFailureError;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;

import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.android.volley.VolleyError;

import com.google.gson.Gson;

import java.util.HashMap;
import java.util.Map;

public class TableList extends Fragment {


    TextView textStatus;
    ImageView imageCongest;
    ImageView imageTable1;
    ImageView imageTable2;

    static RequestQueue requestQueue;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        ViewGroup rootView = (ViewGroup) inflater.inflate(R.layout.table_list, container, false);

        textStatus = (TextView) rootView.findViewById(R.id.textStatus);
        imageCongest = (ImageView) rootView.findViewById(R.id.imageCongest);
        imageTable1 = (ImageView) rootView.findViewById(R.id.imageTable1);
        imageTable2 = (ImageView) rootView.findViewById(R.id.imageTable2);

        Button button = (Button) rootView.findViewById(R.id.button);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
               makeRequest();
            }
        });

        if (requestQueue == null) {
            requestQueue = Volley.newRequestQueue(getActivity());
        }

        return rootView;
    }

    public void makeRequest() {
        String url = "http://54.180.101.233:4885/result";

        StringRequest request = new StringRequest(
                Request.Method.GET,
                url,
                new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {
                       // println("응답 -> " + response);
                        processResponse(response);
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                    //    println("에러 -> " + error.getMessage());
                    }
                }
        ) {
            @Override
            protected java.util.Map<String, String> getParams() throws AuthFailureError {
                Map<String,String> params = new HashMap<String,String>();

                return params;
            }
        };
        request.setShouldCache(false);
        requestQueue.add(request);
       // println("요청 보냄.");
    }

    public void println(String data) {
        textStatus.append(data + "\n");
    }

    public void processResponse(String response) {
        Gson gson = new Gson();
        Table table = gson.fromJson(response, Table.class);

        //// 좌석 혼잡도 ////
        int seatSum = 4;
        int seatUsing = 0;
        if (Integer.parseInt(table.C01T01S01) < 2)
            seatUsing += Integer.parseInt(table.C01T01S01);
        if (Integer.parseInt(table.C01T01S02) < 2)
            seatUsing += Integer.parseInt(table.C01T01S02);
        if (Integer.parseInt(table.C01T02S01) < 2)
            seatUsing += Integer.parseInt(table.C01T02S01);
        if (Integer.parseInt(table.C01T02S02) < 2)
            seatUsing += Integer.parseInt(table.C01T02S02);

        float congest = seatUsing / (float)seatSum;

        if (congest <= 0.1)
            imageCongest.setImageResource(R.drawable.congest_sig_01);
        else if (congest > 0.1 && congest <= 0.2)
            imageCongest.setImageResource(R.drawable.congest_sig_02);
        else if (congest > 0.2 && congest <= 0.3)
            imageCongest.setImageResource(R.drawable.congest_sig_03);
        else if (congest > 0.3 && congest <= 0.4)
            imageCongest.setImageResource(R.drawable.congest_sig_04);
        else if (congest > 0.4 && congest <= 0.5)
            imageCongest.setImageResource(R.drawable.congest_sig_05);
        else if (congest > 0.5 && congest <= 0.6)
            imageCongest.setImageResource(R.drawable.congest_sig_06);
        else if (congest > 0.6 && congest <= 0.7)
            imageCongest.setImageResource(R.drawable.congest_sig_07);
        else if (congest > 0.7 && congest <= 0.8)
            imageCongest.setImageResource(R.drawable.congest_sig_08);
        else if (congest > 0.8 && congest <= 0.9)
            imageCongest.setImageResource(R.drawable.congest_sig_09);
        else
            imageCongest.setImageResource(R.drawable.congest_sig_10);


        //// 테이블 1 ////
        // 현황
        if (table.C01T01S01 == "1" && table.C01T01S02 == "1")
            imageTable1.setImageResource(R.drawable.table_on2);
        else if (table.C01T01S01 == "1" || table.C01T01S02 == "1")
            imageTable1.setImageResource(R.drawable.table_on1);
        else if (table.C01T01S01 == "0" && table.C01T01S02 == "0")
            imageTable1.setImageResource(R.drawable.table_off);
        else if (table.C01T01S01 == "2" && table.C01T01S02 == "2")
            imageTable1.setImageResource(R.drawable.table_null);

        //// 테이블 2 ////
        // 현황
        if (table.C01T02S01 == "1" && table.C01T02S02 == "1")
            imageTable2.setImageResource(R.drawable.table_on2);
        else if (table.C01T02S01 == "1" || table.C01T02S02 == "1")
            imageTable2.setImageResource(R.drawable.table_on1);
        else if (table.C01T02S01 == "0" && table.C01T02S02 == "0")
            imageTable2.setImageResource(R.drawable.table_off);
        else if (table.C01T02S01 == "2" && table.C01T02S02 == "2")
            imageTable2.setImageResource(R.drawable.table_null);

        println(table.time + "\t//" + table.C01T01S01 + ',' + table.C01T01S02 +
        " / " + table.C01T02S01 + ',' + table.C01T02S02);
    }

}
