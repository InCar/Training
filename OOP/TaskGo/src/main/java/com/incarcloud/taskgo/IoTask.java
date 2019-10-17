package com.incarcloud.taskgo;

import com.incarcloud.lang.Action;
import org.apache.http.client.methods.*;
import org.apache.http.impl.client.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;

class IoTask implements Action<String> {
    private static Logger s_logger = LoggerFactory.getLogger(IoTask.class);

    @Override
    public void run(String url) {
        HttpGet httpGet = new HttpGet(url);
        try {
            CloseableHttpClient client = HttpClients.createDefault();
            CloseableHttpResponse response = client.execute(httpGet);
            s_logger.info(response.getStatusLine().toString());
            response.close();
            client.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
