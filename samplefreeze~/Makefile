name ?= samplefreeze~

all: $(name)

$(name):
	gcc -I/opt/purr-data-v2.5.0/include/pd-l2ork "-DNAME=\"$(name)\"" \
		-O2 -fpic -shared -std=c11 main.c -o "$(name).pd_linux"

clean:
	rm -f -- "$(name).pd_linux"
