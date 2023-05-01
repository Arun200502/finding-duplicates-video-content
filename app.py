from flask import Flask, request, render_template
from content_id import generate_id
import subprocess
import os
app = Flask(__name__)
video_names = []
@app.route('/')
def index():
    return render_template('index.html')

@app.route('/uploaded', methods=['POST'])
def process_videos():
    user_id=[]
    totalvalue = request.form['te']
    totalvalue=int(totalvalue)
    totalvalue+=1
    for i in range(1, totalvalue):
        video = request.files.get(f'video{i}')
        if video:
            uploaded_file = request.files[f'video{i}']
            uploaded_file.save('static/uploads/' + uploaded_file.filename)
            video_names.append(video.filename)
            user_id.append(i)
    count = 0
    ids = []
    for i in video_names:
        count+=1
        file_path="static/uploads/"+i
        with open(file_path, 'rb') as f:
            video_contents = f.read()

        file1 = generate_id(video_contents)
        if count == 1:
            with open('hash.txt', 'w') as f:
                f.write(str(file1) + '\n')
            with open('userid.txt', 'w') as f2:
                f2.write(str(count) + '\n')
        else:
            with open('hash.txt', 'a') as f:
                f.write(str(file1) + '\n')
            with open('userid.txt', 'a') as f2:
                f2.write(str(count) + '\n')        
    return render_template('result1.html', video_names=video_names,l=len(video_names))

@app.route('/result', methods=['POST'])
def final():
    os.system("g++ -fopenmp 0804.cpp -o output")
    os.system("output.exe")
    final_hash=[]
    final_user=[]
    with open('hashout.txt', 'r') as file:
        line = file.readline()
        while line:
            final_hash.append(line)
            line = file.readline()
    with open('useridout.txt', 'r') as file:
        line = file.readline()
        while line:
            final_user.append(line)
            line = file.readline()
    final_video_names=[]
    for i in final_user:
        a=int(i)-1
        final_video_names.append(video_names[a])
    return render_template('result2.html', video_names=final_video_names,l=len(final_hash),user_names=final_user)


if __name__== '_main_':
    app.run(debug=True)