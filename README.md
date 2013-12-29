autodeathd
==========

A daemon that monitors udev for suspicious hardware events and takes appropriate action.

The primary use case  environment for this tool would be one where a machine with access to encrypted media is running unsupervised. A casual adversary may attempt to plug in a keyboard or some other hardware device. Autodeathd will detect such attempts and take action to protect the encrypted data.
