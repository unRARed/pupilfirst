module type Error = {
  type t;
  let notification: t => (string, string);
};

module Make = (Error: Error) => {
  exception Errors(array(Error.t));

  let handler = () =>
    [@bs.open]
    (
      fun
      | Errors(errors) =>
        errors
        |> Array.iter(error => {
             let (title, message) = Error.notification(error);
             Notification.error(title, message);
           })
    );

  let catch = (callback, promise) =>
    promise
    |> Js.Promise.catch(error => {
         switch (error |> handler()) {
         | Some(_x) => callback()
         | None => ()
         };
         Js.Promise.resolve();
       });
};