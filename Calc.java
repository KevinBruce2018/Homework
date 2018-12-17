package 计算器;
import java.awt.event.*;
import java.awt.*;
import javax.swing.*;

public class Calc extends JPanel implements ActionListener{
	//声明所有的按钮 显示窗口 标志变量
	private JButton number[] = new JButton[10],operater[] = new JButton[10];
    private JTextField text;
    private JPanel temp[] = new JPanel[4];
    private String op[] = {"+","---","*","/","..","C","<--","平方","开方","="};
    private String num="",num2 = "";
    private int flag = 0;//用于标识当前正在进行的操作
    public Calc()
    {
    	//设置布局以及利用按钮数组添加组件
        this.setLayout(new GridLayout(5, 1));
        text = new JTextField();
        this.add(text);
        text.setEditable(false);
        for(int i = 0;i < 4;i++)
        	temp[i] = new JPanel(new GridLayout(1,5));
		for(int j = 0;j <5;j++) {
			number[j] = new JButton(""+j);
			temp[0].add(number[j]);
			number[j].addActionListener(this);
			operater[j] = new JButton(op[j]);
			operater[j].addActionListener(this);
			temp[2].add(operater[j]);
		}
		for(int j = 5;j <10;j++) {
			number[j] = new JButton(""+j);
			number[j].addActionListener(this);
			temp[1].add(number[j]);
			operater[j] = new JButton(op[j]);
			operater[j].addActionListener(this);
			temp[3].add(operater[j]);
		}
        for(int i = 0;i < 4;i++)
        	this.add(temp[i]);
        this.setVisible(true);
    }
    //处理各种按钮和显示窗口相关的事件
    public void actionPerformed(ActionEvent ev)
    {
    	for(int i = 0;i < 10;i++)
    	{
    		//当触发数字时
    		if(ev.getSource()==number[i])
    		{
    			if(flag==0) {
    				num = num +(""+i);
    				text.setText(num);
    			}
    			else if(flag==-1)
    			{
    				flag = 0;
    				num = ""+i;
    				text.setText(num);
    			}
    			else
    			{
    				num2 = num2+""+i;
    				text.setText(num2);
    			}
    		}
    	}
    	for(int i = 0;i < 4;i++) //触发了加减乘除操作 用标志变量进行标识
    		if(ev.getSource()==operater[i])
    			flag = i+1;
    	if(ev.getSource()==operater[4])//触发了小数点 当点击多次时只出现一个
    	{
    		if(flag==0 ||flag==-1) {
    			if(num.indexOf('.')==-1) {
    				flag = 0;
					num = num +".";
					text.setText(num);
    			}
			}
			else
			{
				if(num2.indexOf('.')==-1) {
					num2 = num2 +".";
					text.setText(num2);
    			}
			}
    	}
    	if(ev.getSource()==operater[5]) //清空当前所有字符
    	{
    		if(flag==0 || flag == -1) {
	    		num = "0";
	    		flag = -1;
	    		text.setText(num);
    		}
    		else
    		{
    			num2 = "0";
	    		text.setText(num2);
    		}
    	}
    	if(ev.getSource()==operater[6])//根据标记的不同每次分情况清空一个字符
    	{
    		if((flag==0 || flag==-1)&&num.length()>0) {
    			flag = 0;
    			num = num.substring(0, num.length()-1);
    			if(num=="")
    				num = "0";
    			text.setText(num);
    		}
    		if(flag!=0 &&flag!=-1&&num2.length()>0) {
    			num2 = num2.substring(0, num2.length()-1);
    			if(num2=="")
    				num2 = "0";
    			text.setText(num2);
    		}
    	}
    	
    	if(ev.getSource()==operater[7]) //平方运算
    	{
    		double rlt;
    		if(flag == 0 ||flag == -1)
    		{
    			if(flag==-1)	flag = 0;
    			rlt = Double.parseDouble(num);
    			num = String.format("%.4f", rlt*rlt);
    		}
    		else
    			rlt = Double.parseDouble(num2);
    		rlt = rlt*rlt;
			text.setText(String.format("%.4f", rlt));
    	}
    	if(ev.getSource()==operater[8])//开方运算
    	{
    		double rlt=0;
    		if(flag == 0 ||flag == -1)
    		{
    			if(flag==-1)	flag = 0;
    			rlt = Math.sqrt(Double.parseDouble(num));
    			num = String.format("%.4f", rlt);
    		}
    		else
    		{
    			rlt = Math.sqrt(Double.parseDouble(num2));
    			num2 = String.format("%.4f", rlt);
    		}
    		text.setText(String.format("%.4f", rlt));
    	}
    	int mark = 0;
    	if(ev.getSource()==operater[9]) //触发了等号 进行求值运算
    	{
    		if(flag!=-1 && flag!=0) {
	    		double n1 = Double.parseDouble(num);
	    		double n2 = Double.parseDouble(num2);
	    		double result = 0;
	    		if(flag==1)
	    			result = n1+n2;
	    		else if(flag==2)
	    			result = n1-n2;
	    		else if(flag==3)
	    			result = n1*n2;
	    		else if(flag==4) {
	    			try {//处理除数为0的操作
	    				if(n2==0)
	    					throw new Exception();
	    				else
	    					result = n1/n2;
	    			}
	    			catch (Exception e) {
						text.setText("0不能做除数");
						mark = 1;
					}
	    		}
	    		//除数不为0，或者进行的是其他操作时进行的操作
	    		flag = -1;
	    		num2 = "";
	    		if(mark!=1) {//除数不为0
	    			num = String.format("%.4f", result);
	    			text.setText(num);
	    		}
	    		else//除数为0
	    		{
	    			flag = 0;
	    			num = "0";
	    		}
    		} 		
    	}
    }
}