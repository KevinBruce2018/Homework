from flask import Flask,render_template,send_from_directory
from flask import request,redirect,session,abort
import os
import pymysql
import config
import hashlib
import re
import time
import random
from tools import *

app = Flask(__name__,static_folder='assets')
app.config['SECRET_KEY'] = 'xxx_is_a_strong_man'

@app.route('/')
def index():
    return render_template('index.html')
@app.route('/login',methods=["POST"])
def login():
    try:
        conn = pymysql.connect(host=config.host,port=config.port,
        user=config.user,passwd=config.password,db=config.db_name
        )
    except:
        return "数据库连接出错！"
    else:
        cursor = conn.cursor()
        sql = "select * from users where username='"+request.form.get('username','').replace("'",'')+"'"
        cursor.execute(sql)
        result = cursor.fetchone()
        password = request.form.get('password','')
        password = hashlib.md5(password.encode())
        password = password.hexdigest()
        if result!=None:
            correct_pass = result[1]
        else:
            correct_pass = 'wrong'
        conn.close()
        if password==correct_pass:
            session['username'] = request.form.get('username','guest')
            if session['username']=='admin':
                return redirect('admin')
            else:
                return redirect('users/'+session['username'])
        else:
            return """
            <script>alert("用户名或密码错误！");
            window.history.go(-1);
            </script>
            """
@app.route('/register',methods=['POST'])
def register():
    user = request.form.get('username','')
    password = request.form.get('password','')
    try:
        conn = pymysql.connect(host=config.host,port=config.port,
        user=config.user,passwd=config.password,db=config.db_name
        )
    except:
        return "数据库连接失败"
    else:
        cursor = conn.cursor()
        sql = "insert into users(username,password) values ('{}','{}');"
        password = hashlib.md5(password.encode()).hexdigest()
        cursor.execute(sql.format(user,password))
        conn.commit()
        cursor.close()
        conn.close()
        if not os.path.exists('uploads/'+user):
            os.mkdir('uploads/'+user)
        return """<script>alert('注册成功!');
        window.location.href='/';
        </script>
        """
@app.route('/regpage')
def reg():
    return render_template('reg.html')
@app.route('/upload',methods=["POST"])
def upload():
    t = str(int(time.time()))
    salt = '@##^*GDJSKDdfk'
    files = request.files['file']
    sandbox = session.get('username','')
    #对用户身份进行鉴权
    if sandbox=='':
        return abort(403)
    if not os.path.exists('uploads/'+sandbox):
        os.mkdir('uploads/'+sandbox)
    #对文件名重命名
    filename = t+salt+str(random.random())
    filename = filename.encode()
    filename = hashlib.md5(filename).hexdigest()
    files.save('uploads/'+sandbox +'/'+ filename+'.'+files.filename.split('.')[-1])
    rlt = save_hash('uploads/'+sandbox +'/'+ filename+'.'+files.filename.split('.')[-1])
    if not rlt:
        return "上传失败"
    return """<script>alert('上传成功!');
        window.location.href='/users/"""+sandbox+"""';
        </script>
        """

@app.route('/uploadpage')
def uploadpage():
    return render_template('upload.html')

@app.route('/checkpage')
def checkpage():
    return render_template('check.html')

@app.route('/download')
def downfile():
    filename = request.args.get('filename','')
    user = base64.b16decode(request.args.get('user','')).decode()
    t = request.args.get('t','')
    sign = request.args.get('sign','')
    salt = 'sfj;fakd;a'
    sign_check = hashlib.md5((t+salt+filename+user).encode()).hexdigest()
    now = int(time.time())
    if now-int(t)>3600:
        return """文件已过期"""
    elif sign_check!=sign:
        return "签名不正确"
    if filename!='':
        return send_from_directory("uploads/"+user,filename,as_attachment=True)
    else:
        return '参数输入错误'

@app.route('/users/<name>')
def users(name):
    role = session.get('username','')
    if role=='':
        return """
            <script>alert("请先进行登录！");
            window.location.href='/';
            </script>
            """
    elif role!=name:
        return """
            <script>alert("身份不匹配，请不要尝试攻击平台！");
            window.location.href='/';
            </script>
            """
    mess = getMessage(name)
    return render_template('user.html',username=name,mess=mess)
@app.route('/logout')
def logout():
    if session.get('username'):
        session.pop('username')
    return """
            <script>alert("退出成功！");
            window.location.href='/';
            </script>
            """

@app.route('/check',methods=["POST"])
def ckeck():
    files = request.files['file']
    sandbox = session.get('username','')
    #对用户身份进行鉴权
    if sandbox=='':
        return abort(403)
    filename = files.filename
    files.save(filename)
    f = open(filename,'rb')

    rlt = check_file(filename,f)
    f.close()
    os.remove(filename)
    if not rlt:
        return """<script>alert('文件有问题!');
        window.location.href='/users/"""+sandbox+"""';
        </script>
        """
    return """<script>alert('文件内容正常!');
        window.location.href='/users/"""+sandbox+"""';
        </script>
        """
@app.route('/resetpwd')
def reset():
    return render_template('changepwd.html')


@app.route('/changepwd',methods=['POST'])
def passwd():
    user = session.get('username','')
    if user=='':
        return abort(403)
    try:
        conn = pymysql.connect(host=config.host,port=config.port,
        user=config.user,passwd=config.password,db=config.db_name
        )
    except:
        return "数据库连接出错！"
    else:
        cursor = conn.cursor()
        sql = "update users set password='"+hashlib.md5(request.form.get('newpwd').encode()).hexdigest()+"' where username='"+session['username']+"'"
        cursor.execute(sql)
        conn.commit()
        cursor.close()
        conn.close()
        if session.get('username'):
            session.pop('username')
        return """<script>alert('密码修改成功,请重新登录!');
        window.location.href='/';
        </script>
        """
@app.route('/delete')
def delete():
    user = session.get('username','')
    if user=='':
        return abort(403)
    filename = 'uploads/'+user+'/'+request.args.get('filename','')
    if os.path.exists(filename):
        os.remove(filename)
    return """<script>alert('文件删除成功!');
        window.location.href='/users/"""+session['username']+"""';
        </script>
        """
@app.route('/admin')
def admin():
    role = session.get('username','')
    if role!='admin':
        return """
            <script>alert("您不是管理员！");
            window.location.href='/';
            </script>
            """
    dirs = os.listdir('uploads')
    mess= []
    for name in dirs:
        tmp = getMessage(name)
        for i in tmp:
            i[3] = i[3].replace('/','')+' '
            i.append('/deluser?user='+name)
            mess.append(i)
    return render_template('admin.html',mess=mess)

@app.route('/deluser')
def deluser():
    user = request.args.get('user','')
    if session.get('username','')!='admin':
        return abort(403)
    if os.path.exists('uploads/'+user):
        os.system('rm -rf uploads/'+user)
    try:
        conn = pymysql.connect(host=config.host,port=config.port,
        user=config.user,passwd=config.password,db=config.db_name
        )
    except:
        return "数据库连接失败"
    else:
        cursor = conn.cursor()
        sql = "delete from users where username='"+user+"'"
        cursor.execute(sql)
        conn.commit()
        cursor.close()
        conn.close()
    return """
            <script>alert("操作成功！");
            window.location.href='/admin';
            </script>
            """
if __name__=='__main__':
    app.run(debug=True,port=5000,host='localhost')