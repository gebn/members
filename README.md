# members

A patched drop-in replacement of [`members(1)`](https://linux.die.net/man/1/members), a tool for finding users in a group.

## Examples

    $ members audio
    speech-dispatcher pulse george
    $ members -p audio
    speech-dispatcher
    $ members -s audio
    pulse george
    $ members -t audio
    speech-dispatcher
    pulse george

## Motivation

I started using `members(1)` to help verify an OpenLDAP directory, but its output did not seem plausible. I took a look at the source, and noticed a glaring bug causing the first primary member of a group (in terms of `getent passwd`) to always be missed out:

    if(theGroup)
      {
        struct passwd *thePasswd;
  
        setpwent();
  
        thePasswd = getpwent();
  
        if(thePasswd)
      {   
        thePasswd = getpwent();  // overwrites the first entity
        
        while(thePasswd)
          {
            // ...
            
            thePasswd = getpwent();
          }
      }
  
        endpwent();
  
        // ...
      }

Instead of creating a patch, I realised I'd stumbled across a languishing package not modified since 2008, with a man page not touched since 1999. It was 363 lines of C++ written in the style of C, with negative indentation and lots of unnecessary complexity (especially with regard to options parsing). So I rewrote it.

This repository is the result. I used pure, modern C to match the underlying APIs (`pwd.h`, `grp.h`). Ignoring documentation (which the original was heavily lacking), code footprint has simultaneously been reduced by 35% to 235 lines.

## Usage

    $ members --help
    Usage: members [OPTION]... GROUP
    Show members of a group.

    members(1) is the complement of groups(1): where groups(1) shows the groups a
    user belongs to, members(1) shows users belonging to a group.

      -p, --primary              show only members for which GROUP is their
                                   primary group
      -s, --secondary            show only members who have a secondary
                                   membership in GROUP
      -t, --two-lines            show primary members on the first line of output
                                   and secondary members on the second line; two
                                   lines are always printed
      -h, --help                 display this help and exit
      -v, --version              output version information and exit

    Exit status:
     0  OK
     1  input error (e.g. no GROUP, unrecognised flag)
     2  GROUP does not exist
     3  GROUP exists but could not be opened (see error message)
     4  GROUP did not have any members
