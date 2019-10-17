package com.incarcloud.taskgo;

import com.incarcloud.concurrent.*;
import com.incarcloud.lang.*;
import org.slf4j.*;
import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;

import java.util.ArrayList;
import java.util.concurrent.Callable;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicReference;

public class App implements CommandLineRunner {
    private static Logger s_logger = LoggerFactory.getLogger(App.class);
    public static void main(String[] args){
        SpringApplication.run(App.class);
    }

    private AtomicReference<LimitedTask> _atomicRef = new AtomicReference<>(null);
    private ScheduledExecutorService _scheduler;

    @Override
    public void run(String... args) {
        beginMetric(1);
        {
            LimitedTask taskRunner;
            Action2<LimitedTask, Integer> actionWait = (runner, max)->{
                runner.setMax(max);
                _atomicRef.set(runner);
                runner.stop();
                print();
            };

            taskRunner = cpu();
            actionWait.run(taskRunner, 2);

            taskRunner = io();
            actionWait.run(taskRunner, 4);

            taskRunner = asyncIo();
            actionWait.run(taskRunner, 4);
        }
        endMetric();
        _asyncIoTask.shutdown();
    }

    private LimitedTask cpu(){
        CpuTask cpuTask = new CpuTask();
        LimitedSyncArgTask<Integer> taskRunner = new LimitedSyncArgTask<>(cpuTask);
        taskRunner.setMax(4);
        for(int i=0;i<1024;i++) {
            taskRunner.submit(512);
        }

        return taskRunner;
    }

    private LimitedTask io(){
        IoTask ioTask = new IoTask();
        LimitedSyncArgTask<String> taskRunner = new LimitedSyncArgTask<>(ioTask);
        for(int i=0;i<32;i++) {
            taskRunner.submit("https://www.baidu.com");
        }

        return taskRunner;
    }

    private AsyncIoTask _asyncIoTask = new AsyncIoTask();
    private LimitedTask asyncIo(){
        LimitedAsyncArgTask<String> taskRunner = new LimitedAsyncArgTask<String>(_asyncIoTask);
        for(int i=0;i<32;i++) {
            taskRunner.submit("https://www.baidu.com");
        }

        return taskRunner;
    }

    private void beginMetric(int interval){
        _scheduler = Executors.newSingleThreadScheduledExecutor();
        _scheduler.scheduleAtFixedRate(()->{ print(); }, interval, interval, TimeUnit.SECONDS);
    }

    private void endMetric()
    {
        _scheduler.shutdown();
    }

    private void print(){
        LimitedTask taskRunner = _atomicRef.get();
        if (taskRunner != null) {
            String metric = LimitedTask.printMetric(taskRunner, 0);
            s_logger.info("\n" + metric);
        }
    }
}
