from tkinter import *
from tkinter.ttk import * 
from scapy.all import *

class sniffThread (threading.Thread):  
    def __init__(self, threadID, name):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.name = name
    def run(self):
        try:
            sniff(iface=None if not ifaceCom.get() else ifaceCom.get() ,filter=eFliter.get(),prn=prn)
        except:
            pass

cnt = 0
thread1 = None
dataList = []

#中断进程的方法
def _async_raise(tid, exctype):
    tid = ctypes.c_long(tid)
    if not inspect.isclass(exctype):
        exctype = type(exctype)
    res = ctypes.pythonapi.PyThreadState_SetAsyncExc(tid, ctypes.py_object(exctype))
    if res == 0:
        raise ValueError("invalid thread id")
    elif res != 1:
        ctypes.pythonapi.PyThreadState_SetAsyncExc(tid, None)
        raise SystemError("PyThreadState_SetAsyncExc failed")


def start():
    global thread1,cnt
    # 如果线程仍在运行则中断
    if isinstance(thread1,sniffThread):
        stop()
    lPackageList.delete(0,last=cnt)
    cnt = 0
    dataList.clear()
    thread1 = sniffThread(1, "sniff")
    thread1.start()

def stop():
    global thread1
    if not isinstance(thread1,sniffThread):
        return
    _async_raise(thread1.ident, SystemExit)
    thread1 = None

def prn(x):
    global cnt
    s = x.summary()
    lPackageList.insert(cnt,str(cnt)+'    '+s)
    dataList.append(x)
    cnt+=1

def showPackge(event):
    if cnt == 0:
        return
    index = lPackageList.curselection()[0]
    tAna.delete("1.0",END)
    tRaw.delete("1.0",END)
    tAna.insert(END,dataList[index].show(dump=True))
    tRaw.insert(END,hexdump(dataList[index],dump=True))

def getIfaceList():
    ls = repr(ifaces).split('\n')
    ls = ls[1:]
    res = []
    for item in ls:
        sls = item.split()
        res.append(' '.join(sls[1:-2]))
    return res

window = Tk()
window.title('网络协议分析器')
window.geometry('980x600')
window.resizable(0,0)
ifaceCom = Combobox(window,width = 45)
lListenIface = Label(window, text='监听网卡:')
lFliter = Label(window, text='过滤条件:')
eFliter =  Entry(window, show=None,width = 45)
bStart = Button(window,text='开始',command = start) 
bStop = Button(window,text = '停止',command = stop)
#数据包列表
lPackageList = Listbox(window,width=135)
lPackageList.bind('<ButtonRelease-1>',showPackge)  #ButtonRelease-1鼠标左键松开

sPackageList = Scrollbar(window,command = lPackageList.yview)
lListenIface.grid(row = 1,column =1)

ifaceCom.grid(row = 1,column =2)
lFliter.grid(row=1,column=3)
eFliter.grid(row=1,column=4)
bStart.grid(row=1,column=5)
bStop.grid(row=1,column=6)

lPackageList.grid(row = 2,column = 1,columnspan = 6)
sPackageList.grid(row = 2,column = 7,sticky = N+S)
lPackageList.config(yscrollcommand=sPackageList.set)

lbAna = Label(window,text = '数据包分析')
tAna = Text(window,width = 135,height = 12)
sAna = Scrollbar(window,command = tAna.yview)

lbRaw = Label(window,text = '原始内容')
tRaw = Text(window,width = 135,height = 12)
sRaw = Scrollbar(window,command = tRaw.yview)

lbAna.grid(row=3,column=1)
tAna.grid(row=4,column=1,columnspan = 6)
sAna.grid(row=4,column=7,sticky = N+S)
tAna.config(yscrollcommand=sAna.set)

lbRaw.grid(row=5,column=1)
tRaw.grid(row=6,column=1,columnspan = 6)
sRaw.grid(row=6,column=7,sticky = N+S)
tRaw.config(yscrollcommand=sRaw.set)

ifaceCom['value'] = getIfaceList()
window.mainloop()
