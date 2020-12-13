from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
import sys
from xml.dom.minidom import parse
from xml.dom import minidom

class Student(QWidget):
    def __init__(self):
        super().__init__()
        self.layout = QVBoxLayout()
        self.setWindowTitle('学生信息管理系统')
        self.setGeometry(700,200,750,600)
        la = [QLabel(' ') for i in range(3)]
        hbox = QHBoxLayout()
        btn1 = QPushButton('添加')
        btn1.clicked.connect(self.add)
        #这里触发相应的方法
        #btn.clicked.connect(self.start_request)
        btn2 = QPushButton('删除')
        btn2.clicked.connect(self.delete)
        #self.line = QLineEdit()
        #hbox中添加四个按钮
        btn3 = QPushButton('修改')
        btn3.clicked.connect(self.change)
        btn4 = QPushButton('查询')
        btn4.clicked.connect(self.search)
        label = QLabel('  学号 ')
        self.line = QLineEdit()

        hbox.addWidget(btn1)
        hbox.addWidget(la[0])
        #hbox.addWidget(self.line)
        hbox.addWidget(btn2)
        hbox.addWidget(la[1])
        hbox.addWidget(btn3)
        hbox.addWidget(la[2])
        hbox.addWidget(btn4)
        hbox.addWidget(label)
        hbox.addWidget(self.line)
        w = QWidget()
        w.setLayout(hbox)
        self.layout.addWidget(w)
        self.setLayout(self.layout)

        self.table = QTableWidget()
        self.table.setRowCount(20)
        self.table.setColumnCount(5)
        self.table.setHorizontalHeaderLabels(['姓名','性别','年龄','学号','专业']) 
        
        self.layout.addWidget(self.table)
        self.parse_xml()
        #self.show()
    def add(self):
        domtree = parse('stu.xml')
        #dom = minidom.Document()
        self.child = childWindow(domtree,self.parse_xml)
        self.child.show()
    def delete(self):
        id = self.line.text()
        domtree = parse('stu.xml')
        dom = minidom.Document()
        root = dom.createElement('students')
        student = domtree.getElementsByTagName('student')

        for i in student:
            if i.getElementsByTagName('id')[0].childNodes[0].data !=id:
                root.appendChild(i)
        with open('stu.xml','w',encoding='UTF-8') as f:
            root.writexml(f,indent='',addindent='\t',newl='\n')
        files = '<?xml version="1.0" encoding="UTF-8"?>\n'
        with open('stu.xml','r',encoding='UTF-8') as fh:
            lines = fh.readlines()
        for i in lines:
            if i.rstrip()!='':
                files+=i.rstrip()+'\n'
        with open('stu.xml','w',encoding='UTF-8') as fh:
            fh.write(files)
        self.clear()
        self.parse_xml()
    def change(self):
        self.clear()
        id = self.line.text()
        domtree = parse('stu.xml')
        
        self.change = Change(domtree,self.parse_xml,id)

    def search(self):
        self.clear()
        count = 0
        id = self.line.text()
        domtree = parse('stu.xml')
        dom = minidom.Document()
        root = dom.createElement('students')
        student = domtree.getElementsByTagName('student')
        for i in student:
            if i.getElementsByTagName('id')[0].childNodes[0].data ==id:
                self.table.setItem(count,0,QTableWidgetItem(i.getElementsByTagName('name')[0].childNodes[0].data))
                self.table.setItem(count,1,QTableWidgetItem(i.getElementsByTagName('sex')[0].childNodes[0].data))
                self.table.setItem(count,2,QTableWidgetItem(i.getElementsByTagName('age')[0].childNodes[0].data))
                self.table.setItem(count,3,QTableWidgetItem(i.getElementsByTagName('id')[0].childNodes[0].data))
                self.table.setItem(count,4,QTableWidgetItem(i.getElementsByTagName('major')[0].childNodes[0].data))
                count+=1
    def display(self,rlt):
        for i in range(len(rlt)):
            self.table.setItem(i,0,QTableWidgetItem(rlt[i]['name']))
            self.table.setItem(i,1,QTableWidgetItem(rlt[i]['sex']))
            self.table.setItem(i,2,QTableWidgetItem(rlt[i]['age']))
            self.table.setItem(i,3,QTableWidgetItem(rlt[i]['id']))
            self.table.setItem(i,4,QTableWidgetItem(rlt[i]['major']))
    def clear(self):
        for i in range(20):
            self.table.setItem(i,0,QTableWidgetItem(''))
            self.table.setItem(i,1,QTableWidgetItem(''))
            self.table.setItem(i,2,QTableWidgetItem(''))
            self.table.setItem(i,3,QTableWidgetItem(''))
            self.table.setItem(i,4,QTableWidgetItem(''))
    def parse_xml(self):
        rlt_lists = []
        domtree = parse('stu.xml')
        collection = domtree.documentElement
        students_lists = collection.getElementsByTagName('student')
        for stu in students_lists:
            rlt = {}
            rlt['name'] = stu.getElementsByTagName('name')[0].childNodes[0].data
            rlt['sex'] = stu.getElementsByTagName('sex')[0].childNodes[0].data
            rlt['age'] = stu.getElementsByTagName('age')[0].childNodes[0].data
            rlt['id'] = stu.getElementsByTagName('id')[0].childNodes[0].data
            rlt['major'] = stu.getElementsByTagName('major')[0].childNodes[0].data
            rlt_lists.append(rlt)
        #print(rlt_lists)
        self.display(rlt_lists)
               

class childWindow(QDialog):
    def __init__(self,domtree,fun):
        QDialog.__init__(self)
        #注意不要将该窗口设置成临时变量，否则会一闪而过
        self.domtree = domtree
        self.fun = fun
        self.setGeometry(850,350,270,350)
        #self.child.setupUi(self)
        self.setWindowTitle('添加学生信息')
        self.layout = QVBoxLayout()

        self.setLayout(self.layout)
        #姓名 性别 年龄 学号 专业
        self.label_list = [QLabel(' 姓名'),QLabel(' 性别'),QLabel(' 年龄'),QLabel(' 学号'),QLabel(' 专业')]
        self.line_list = [QLineEdit() for i in range(5)]
        self.btn = QPushButton('提交')
        for i in range(5):
            layouts = QHBoxLayout()
            layouts.addWidget(self.label_list[i])
            layouts.addWidget(self.line_list[i])
            #
            w = QWidget()
            w.setLayout(layouts)
            self.layout.addWidget(w)
        layouts = QHBoxLayout()
        w = QWidget()
        w.setLayout(layouts)
        layouts.addWidget(QLabel('  '))
        layouts.addWidget(self.btn)
        self.btn.clicked.connect(self.submit)
        self.layout.addWidget(w)
        self.show()
        
    def submit(self):
        dom = minidom.Document()
        student = dom.createElement('student')
        name = dom.createElement('name')
        age = dom.createElement('age')
        sex = dom.createElement('sex')
        id = dom.createElement('id')
        major = dom.createElement('major')
        text = []
        for i in range(5):
            text.append(self.line_list[i].text())
        name.appendChild(dom.createTextNode(text[0]))
        age.appendChild(dom.createTextNode(text[1]))
        sex.appendChild(dom.createTextNode(text[2]))
        id.appendChild(dom.createTextNode(text[3]))
        major.appendChild(dom.createTextNode(text[4]))
        student.appendChild(name)
        student.appendChild(age)
        student.appendChild(sex)
        student.appendChild(id)
        student.appendChild(major)
        root = self.domtree.getElementsByTagName('students')[0]
        root.appendChild(student)
        with open('stu.xml','w',encoding='UTF-8') as fh:
            root.writexml(fh,indent='',addindent='\t',newl='\n')
        files = '<?xml version="1.0" encoding="UTF-8"?>\n'
        with open('stu.xml','r',encoding='UTF-8') as fh:
            lines = fh.readlines()
        for i in lines:
            if i.rstrip()!='':
                files+=i.rstrip()+'\n'
        with open('stu.xml','w',encoding='UTF-8') as fh:
            fh.write(files)
        self.fun()
        self.close()
 
class childWindow(QDialog):
    def __init__(self,domtree,fun):
        QDialog.__init__(self)
        #注意不要将该窗口设置成临时变量，否则会一闪而过
        self.domtree = domtree
        self.fun = fun
        self.setGeometry(850,350,270,350)
        #self.child.setupUi(self)
        self.setWindowTitle('添加学生信息')
        self.layout = QVBoxLayout()

        self.setLayout(self.layout)
        #姓名 性别 年龄 学号 专业
        self.label_list = [QLabel(' 姓名'),QLabel(' 性别'),QLabel(' 年龄'),QLabel(' 学号'),QLabel(' 专业')]
        self.line_list = [QLineEdit() for i in range(5)]
        self.btn = QPushButton('提交')
        for i in range(5):
            layouts = QHBoxLayout()
            layouts.addWidget(self.label_list[i])
            layouts.addWidget(self.line_list[i])
            #
            w = QWidget()
            w.setLayout(layouts)
            self.layout.addWidget(w)
        layouts = QHBoxLayout()
        w = QWidget()
        w.setLayout(layouts)
        layouts.addWidget(QLabel('  '))
        layouts.addWidget(self.btn)
        self.btn.clicked.connect(self.submit)
        self.layout.addWidget(w)
        self.show()
        
    def submit(self):
        dom = minidom.Document()
        student = dom.createElement('student')
        name = dom.createElement('name')
        age = dom.createElement('age')
        sex = dom.createElement('sex')
        id = dom.createElement('id')
        major = dom.createElement('major')
        text = []
        for i in range(5):
            text.append(self.line_list[i].text())
        name.appendChild(dom.createTextNode(text[0]))
        age.appendChild(dom.createTextNode(text[1]))
        sex.appendChild(dom.createTextNode(text[2]))
        id.appendChild(dom.createTextNode(text[3]))
        major.appendChild(dom.createTextNode(text[4]))
        student.appendChild(name)
        student.appendChild(age)
        student.appendChild(sex)
        student.appendChild(id)
        student.appendChild(major)
        root = self.domtree.getElementsByTagName('students')[0]
        root.appendChild(student)
        with open('stu.xml','w',encoding='UTF-8') as fh:
            root.writexml(fh,indent='',addindent='\t',newl='\n')
        files = '<?xml version="1.0" encoding="UTF-8"?>\n'
        with open('stu.xml','r',encoding='UTF-8') as fh:
            lines = fh.readlines()
        for i in lines:
            if i.rstrip()!='':
                files+=i.rstrip()+'\n'
        with open('stu.xml','w',encoding='UTF-8') as fh:
            fh.write(files)
        self.fun()
        self.close()
class Change(QDialog):
    def __init__(self,domtree,fun,id):
        QDialog.__init__(self)
        #注意不要将该窗口设置成临时变量，否则会一闪而过
        self.domtree = domtree
        self.fun = fun
        self.id = id
        self.setGeometry(850,350,270,350)
        #self.child.setupUi(self)
        self.setWindowTitle('修改学生信息')
        self.layout = QVBoxLayout()

        self.setLayout(self.layout)
        #姓名 性别 年龄 学号 专业
        self.label_list = [QLabel(' 姓名'),QLabel(' 性别'),QLabel(' 年龄'),QLabel(' 专业')]
        self.line_list = [QLineEdit() for i in range(4)]
        self.btn = QPushButton('提交')
        for i in range(4):
            layouts = QHBoxLayout()
            layouts.addWidget(self.label_list[i])
            layouts.addWidget(self.line_list[i])
            #
            w = QWidget()
            w.setLayout(layouts)
            self.layout.addWidget(w)
        layouts = QHBoxLayout()
        w = QWidget()
        w.setLayout(layouts)
        layouts.addWidget(QLabel('  '))
        layouts.addWidget(self.btn)
        self.btn.clicked.connect(self.submit)
        self.layout.addWidget(w)
        self.show()
        
    def submit(self):
        text = []
        for i in range(4):
            text.append(self.line_list[i].text())
        dom = minidom.Document()
        root = dom.createElement('students')
        student = self.domtree.getElementsByTagName('student')
        index = 0
        for i in student:
            if i.getElementsByTagName('id')[0].childNodes[0].data !=self.id:
                root.appendChild(i)
            else:
                i.getElementsByTagName('name')[0].childNodes[0].data = text[0]
                i.getElementsByTagName('sex')[0].childNodes[0].data = text[1]
                i.getElementsByTagName('age')[0].childNodes[0].data = text[2]
                i.getElementsByTagName('major')[0].childNodes[0].data = text[3]
                root.appendChild(i) 
            index+=1
        with open('stu.xml','w',encoding='UTF-8') as fh:
            root.writexml(fh,indent='',addindent='\t',newl='\n')
        files = '<?xml version="1.0" encoding="UTF-8"?>\n'
        with open('stu.xml','r',encoding='UTF-8') as fh:
            lines = fh.readlines()
        for i in lines:
            if i.rstrip()!='':
                files+=i.rstrip()+'\n'
        with open('stu.xml','w',encoding='UTF-8') as fh:
            fh.write(files)
        self.fun()
        self.close()
 
            

app = QApplication(sys.argv)
a = Student()
a.show()

sys.exit(app.exec_())