import java.awt.*;
import java.awt.event.AdjustmentEvent;
import java.awt.event.AdjustmentListener;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;
import java.util.Date;
import java.util.Vector;

import javax.swing.*;

public class ScrollBarDemo {

    public static void main(String[] args) {
        DrawFrame frame = new DrawFrame();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);
    }

}

class DrawFrame extends JFrame {
    private static String txt=buildTxt(50);
    public DrawFrame() {
        setTitle("ScrollBarDemo");
        setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
        // 将 panel 加到 frame
        DrawPanel panel = new DrawPanel();
        Container contentPane = getContentPane();

        Scrollbar scrollBar = new Scrollbar();

        scrollBar.setOrientation(Scrollbar.VERTICAL);
        contentPane.add(panel, BorderLayout.CENTER);

        scrollBar.addAdjustmentListener(new AdjustmentListener() {
            @Override
            public void adjustmentValueChanged(AdjustmentEvent e) {
                panel.repaint(scrollBar.getValue());
            }
        });

        this.addMouseWheelListener(new MouseWheelListener() {
            @Override
            public void mouseWheelMoved(MouseWheelEvent e) {
                scrollBar.setValue(scrollBar.getValue() + e.getWheelRotation());
                panel.repaint(scrollBar.getValue());
            }
        });

        contentPane.add(scrollBar, BorderLayout.EAST);
    }

    public static final int DEFAULT_WIDTH = 800;
    public static final int DEFAULT_HEIGHT = 600;


    class DrawPanel extends JPanel {
        private int scrollBarValue=0;

        public void paintComponent(Graphics g,double leftX,  double topY ) {
            super.paintComponent(g);

            g.setFont(new Font("", Font.ROMAN_BASELINE, 16));

            String[] str=format(txt,this.getWidth(),g);//最大行宽=窗体宽度-6
            drawTxtArray(str,g,0,(int) topY + 10);//起始坐标

            g.drawString("这是最后一行文本 当前字体行高:" + g.getFontMetrics().getHeight()+"  窗体宽度:"+this.getWidth(), 0, this.getHeight()-5 );
        }

        // 字符串切割,实现字符串自动换行

        /**
         * 切割字符串为数组，依据字符在当前字体的对应宽度以及允许的最大行宽度运算
         * @param text 字符串
         * @param maxWidth 最大宽度 根据当前窗体宽度计算
         * @param g Graphics
         * @return
         */
        public  String[] format(String text, int maxWidth, Graphics g) {
            String[] result = null;
            Vector tempR = new Vector();
            int lines = 0;
            int len = text.length();
            int index0 = 0;
            int index1 = 0;
            boolean wrap;
            while (true) {
                int widthes = 0;
                for (index0 = index1; index1 < len; index1++) {
                    widthes = g.getFontMetrics().charWidth(text.charAt(index1)) + widthes;
                    if (widthes > maxWidth) {
                        break;
                    }
                }
                lines++;
                tempR.addElement(text.substring(index0, index1));
                if (index1 >= len) {
                    break;
                }
            }
            result = new String[lines];
            tempR.copyInto(result);
            return result;
        }

        /**
         * 写字符串数组到指定起始坐标
         * @param texts 文本数组
         * @param g Graphics
         * @param x 起始坐标x
         * @param y 起始坐标y
         */
        public void drawTxtArray(String[] texts,Graphics g,int x,int y){
            for(int i=0;i<texts.length;i++){
                if(y+g.getFontMetrics().getHeight()*i<=this.getHeight()-g.getFontMetrics().getHeight()){
                    //确保文本不会绘制到最后一行的保留区域
                    g.drawString(texts[i], 0, y + g.getFontMetrics().getHeight()*i);
                }

            }
        }

        /**
         * 自动调用重绘方法
         * @param g Graphics
         */
        public void paint(Graphics g) {

            System.out.println(new Date().toLocaleString() + " 重绘..." + this.getWidth() + ":" + this.getHeight()+" value:"+scrollBarValue);
            paintComponent(g, 0, 10 - scrollBarValue * 5 );
        }

        /**
         * 重载的repaint方法 使用scrollBarValue控制绘制坐标
         * @param value scrollBarValue
         */
        public void repaint(int value) {

            scrollBarValue=value;
            super.repaint();
        }

    }

    /**
     *  生成文本
     * @param lineLength 文本数
     * @return
     */
    public static String buildTxt(int lineLength){
        StringBuilder sb=new StringBuilder();
        for(int i=0;i < lineLength; i++) {
            //sb.append(new Date().getTime()).append("-").append(i+"t");
            for(int j=65;j<91;j++){
                sb.append((char)j);
            }
        }
        return sb.toString();
    }
}

