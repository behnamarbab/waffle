Command line used to find this crash:

/home/bbojnord/Dropbox/UNB/Thesis/S1/Waffle_Memlock/tool/MemLock/memlock-stack-fuzz -i /home/bbojnord/Dropbox/UNB/Thesis/S1/Waffle_Memlock/tests/test5/in -o /home/bbojnord/Dropbox/UNB/Thesis/S1/Waffle_Memlock/tests/test5/out_MemLock1 -m none -d -t 5000 -- /home/bbojnord/Dropbox/UNB/Thesis/S1/Waffle_Memlock/tests/test5/example5Waffle @@

If you can't reproduce a bug outside of afl-fuzz, be sure to set the same
memory limit. The limit used for this fuzzing session was 0 B.

Need a tool to minimize test cases before investigating the crashes or sending
them to a vendor? Check out the afl-tmin that comes with the fuzzer!

Found any cool bugs in open-source tools using afl-fuzz? If yes, please drop
me a mail at <lcamtuf@coredump.cx> once the issues are fixed - I'd love to
add your finds to the gallery at:

  http://lcamtuf.coredump.cx/afl/

Thanks :-)
