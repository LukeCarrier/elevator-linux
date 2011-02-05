Elevator
========

This is a project I started when I was working on a couple of daemons (background applications) for a server management utility. It tackles the problem with running a daemon as limited user and running certain commands as root.

In effect, all Elevator does is launch another application. Alone, it may not elevate the permissions of the application launches, since it relies upon the Linux filesystem's setuid bit. For those not familiar with this bit, it determines wwhether the application should be run as the user who owns the file, or the user who called the file.

Warning
-------

As with all tools of its type, Elevator is susceptible to abuse if it's not configured correctly. Ensure that the user who is designated as able to use the application (the 'allowed user') isn't accessible to non-authorised people, or they'll be able to run whatever commands they please with root privileges. That's bad ;-)

Although I attempt to ensure that the code within is as stable as possible, I'm only human and I do make mistakes. I cannot, and will not, accept responsiblity for any damage this utility might cause on your machine, though I'm always happy to help.
