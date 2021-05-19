#进行hash校验和入库操作,传入文件的路径
import config
import hashlib
import pymysql
import os
import time
import base64
def save_hash(filename):
    try:
        conn = pymysql.connect(host=config.host,port=config.port,
        user=config.user,passwd=config.password,db=config.db_name
        )
    except:
        return False
    else:
        f = open(filename,'rb')
        hex_str = hashlib.sha1(f.read()).hexdigest()
        f.close()
        cursor = conn.cursor()
        sql = "insert into hash(name,hash) values ('{}','{}');"
        filename = filename.split('/')[-1]
        cursor.execute(sql.format(filename,hex_str))
        conn.commit()
        cursor.close()
        conn.close()
        return True
def check_file(filename,f):
    try:
        conn = pymysql.connect(host=config.host,port=config.port,
        user=config.user,passwd=config.password,db=config.db_name
        )
    except:
        return False
    else:
        hex_str = hashlib.sha1(f.read()).hexdigest()
        cursor = conn.cursor()
        sql = "select hash from hash where name='"+filename+"';"
        cursor.execute(sql)
        rlt = cursor.fetchone()
        if rlt!=() and rlt!=None and rlt[0]==hex_str:
            return True
        cursor.close()
        conn.close()
        return False
def getLink(filename,user):
    t = str(int(time.time()))
    salt = 'sfj;fakd;a'
    sign = hashlib.md5((t+salt+filename+user).encode()).hexdigest()
    user = base64.b16encode(user.encode()).decode()
    return 'http://127.0.0.1:5000/download?filename='+filename+'&user='+user+'&t='+t+'&sign='+sign
def getMessage(username):
    dirs = os.listdir('uploads/'+username)
    result = []
    for i in range(len(dirs)):
        ids = '{:05d}'.format(i+1)
        file_size = os.path.getsize('uploads/'+username+'/'+dirs[i])
        direct = 'uploads/'+username+'/'+dirs[i]
        f = open(direct,'rb')
        hash_code = hashlib.sha1(f.read()).hexdigest()
        f.close()
        
        link = getLink(dirs[i],username)
        
        tmp = [ids,dirs[i],file_size,'/'+username,hash_code,link,'/delete?filename='+dirs[i]]
        result.append(tmp)
    return result
def encrypt(oldpwd,newpwd):
    oldpwd = base64.b64encode(oldpwd.encode())
    newpwd = base64.b64encode(newpwd.encode())
    oldpwd = [chr(ord(i)+3) for i in oldpwd]
    newpwd = [chr(ord(i)+3) for i in newpwd]
    return oldpwd,newpwd