#images to video
# -r fps
# -crf quiality less better 25 is ok
ffmpeg -r 29.97 -f image2 -s 720x480 -i 01/01_%07d.jpg -vcodec libx264 -crf 25 -pix_fmt yuv420p 01.mp4
#extract audio
ffmpeg -i /media/papobackup/backup/video/허준/hur-jun-episode-1.mp4 -vn -acodec copy audio01.aac
#merge audio video
ffmpeg -i 01.mp4 -i audio01.aac -c:v copy -c:a aac va01.mp4
#change sar dar
ffmpeg -i va01.mp4 -vf "scale=720:480,setsar=sar=8:9,setdar=dar=4:3" vas01.mp4
