package com.incarcloud.taskgo;

import com.incarcloud.lang.Action2;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.concurrent.FutureCallback;
import org.apache.http.impl.nio.client.*;
import org.slf4j.*;

import java.io.IOException;

class AsyncIoTask implements Action2<String, Runnable>{
    private CloseableHttpAsyncClient _client = HttpAsyncClients.createDefault();

    public AsyncIoTask(){
        _client.start();
    }

    public void shutdown(){
        try {
            _client.close();
        }
        catch (IOException e){

        }
    }

    @Override
    public void run(String url, Runnable onEnd) {
        HttpGet httpGet = new HttpGet(url);
        _client.execute(httpGet, new AsyncIoCallback(onEnd));
    }
}

class AsyncIoCallback implements FutureCallback<HttpResponse> {
    private static Logger s_logger = LoggerFactory.getLogger(AsyncIoCallback.class);
    private Runnable _onEnd;

    AsyncIoCallback(Runnable onEnd){
        _onEnd = onEnd;
    }

    @Override
    public void completed(HttpResponse result) {
        s_logger.info(result.getStatusLine().toString());
        end();
    }

    @Override
    public void failed(Exception ex) {
        end();
    }

    @Override
    public void cancelled() {
        end();
    }

    private void end(){
        _onEnd.run();
    }
}