git remote -v
git fetch origin
cppcheck --version
ls
git branch -a
changed_files=$(git diff --name-only HEAD origin/master | grep -E '\.(c|cpp|cc|cxx)$' || true)
echo "Changed files: $changed_files"
if [ -n "$changed_files" ];then
    cppcheck --enable=warning,performance,portability --inline-suppr --error-exitcode=1 --force $changed_files
    err=$?
    if [ $err -ne 0 ]; then
        echo "CPPCHECK FAILED, PLEASE FIX THE PROBLEMS"
    fi
fi