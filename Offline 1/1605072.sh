#!/bin/bash

if [ $# -ne 1 ]; then
	echo "please run the script as: $0 input_file_name"
	exit
fi


input=$1

<<comment
cnt=0
while IFS= read -r in
do
	line[$cnt]= $in
	echo "$cnt"
	cnt= `expr $cnt + 1`
done < "$input"
comment
<<comment
cnt=0
while IFS= read -r in; do

	line[$cnt]=$in
	echo "$cnt"
	echo "$in"
	cnt=`expr $cnt + 1`

done < "$input"
comment

cnt=0
for((cnt=0;cnt<3;cnt++))
do
	IFS= read -r in
	line[$cnt]=$in
	echo "$cnt"
	echo "$in"
	#cnt=`expr $cnt + 1`

done < "$input"



rm -r ../outputdir
mkdir ../outputdir
output_dir_path="$(realpath ../outputdir)"
root="$(realpath .)"
base_root="${root##*/}"
remove_from_root="${root%/*$base_root}"
#echo $base_root $remove_from_root
#echo $base_root
#echo $remove_from_root


cmd=$(head -n 1 "$input" | tail -n 1)

if [ ${line[0]}="begin" ]; then
cmd="head"
elif [ ${line[0]}="end" ]; then
cmd="tail"

fi

s2="tail"

nol=$(head -n 2 "$input" | tail -n 1)
word_to_search=$(head -n 3 "$input" | tail -n 1)

final_path="$(realpath .)"

nn=0
# first param: directory to go
traverse_directories() {
    cd "$1"
	
	for f in *
	do
		if [ -d "$f" ]; then
			# next level directory
			#final_path="$(realpath .)"
			
			traverse_directories "$f"
		elif [ -f "$f" ]; then
			if file "$f" | grep -q "ASCII\|Unicode" ; then
				# text file; need to process
				if $cmd -n $nol $f | grep -qi $word_to_search; then
					fn="${f%.*}"
					ext="${f##*.}"
					line_no=$($cmd -n $nol $f | grep -ni $word_to_search | cut -f1 -d':'| head -n 1)

					#nl=$(wc -l < $f)
					#echo lines: $nl

					#new_file_name="${fn}_${line_no}.${ext}"
					#dir="${output_dir_path}"

					if [ "$cmd" == "$s2" ];then
						total=$(wc -l < $f)
						cnt=`expr $total - $nol`
						replace=$line_no
						line_no=`expr $replace + $cnt`
					fi

					new="$(realpath ../$f)"
					#dir="${final_path#/home/binoy/Desktop/314/root1/}"
					dir="${new#$remove_from_root}"
					#echo directory: $dir
					nn=`expr $nn + 1`
					
					echo $nn
					prefix=${dir%.*}
					#echo $(realpath .)

					#if($cmd == end then line_no)
					#end na hole ager line_no e thakbe

					final=$(echo "$prefix" | sed -r 's/[/]+/./g')
					
					final="${final##.}"
					#echo here: $final
					#new_file_name="${final}.${fn}${line_no}.${ext}"
					#new_file_name="${final}.${line_no}.${ext}"
					new_file_name="${final}_${line_no}.${ext}"
					echo here2: "${new_file_name}"
					cp "$f" "${output_dir_path}/${new_file_name}"
					#echo here: "${new_file_name}"
					#cp $f "${output_dir_path}/${new_file_name}"
				fi
			else
				echo $f is a non text file
			fi
		fi
		#echo "(2)No. of Files: $nn" 
	done

	#echo "No. of Files: $nn" 

	cd ../
	echo "No. of Files: $nn" 
}



traverse_directories . 