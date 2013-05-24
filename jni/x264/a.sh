f(){  
return 1
}
f2(){
return 0
}
for opt do
	optarg="${opt#*=}"
	echo $optarg
	echo $opt
done
f || echo 'OKA'
f2 || echo 'error  2'
