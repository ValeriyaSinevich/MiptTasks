// Learn more about F# at http://fsharp.org
// See the 'F# Tutorial' project for more help.
#r "C:\Users\user\Documents\Visual Studio 2015\Projects\lastSem\packages\FSharp.Data.2.3.0\lib\net40\FSharp.Data.dll";;
open FSharp.Data
open System
open System.IO
open System.Net
open System.Text
open System.Collections.Specialized

let email = "valeriya.sinevich@phystech.edu"

let equal a b  = (String.Compare(a, b, StringComparison.OrdinalIgnoreCase) = 0)

let language_links list = 
    let start = List.findIndex (fun x-> equal "#See_also" x) list
    let fin = List.findIndex (fun x-> equal "/wiki/Comparison_of_programming_languages" x) list
    list |> List.take fin |> List.rev |> List.take (fin - start - 1) |> List.rev


let getLinks () =
  let langs = HtmlDocument.Load("https://en.wikipedia.org/wiki/List_of_programming_languages")
  langs.Descendants ["ul"] 
    |> Seq.collect (fun (x:HtmlNode) -> x.Descendants ["a"])
    |> Seq.map(fun x -> x.AttributeValue "href") 
    |> Seq.toList
    |> language_links


let getDeveloper (ref : string) : string = 
  let lang = HtmlDocument.Load(ref)
  lang.Descendants ["tr"] 
  |> Seq.map (fun (x:HtmlNode) -> (x.Descendants ["th"], x.Descendants ["td"]))
  |> Seq.map (fun ((x, y) : seq<HtmlNode> * seq<HtmlNode>) -> 
    let attr = x |> Seq.map (fun x -> x.InnerText())  |> Seq.toList
    if (Seq.exists (fun (s : string) -> equal "Designed by" s) attr)
    then (y |> Seq.map(fun x -> x.InnerText()) |> Seq.toList |> List.item 0)
    else "")
  |> Seq.toList
  |> List.filter (fun x -> String.length x > 0)
  |> (fun (x : string list) -> if List.length x > 0 then List.item 0 x else "")

let getDeveloperSafe x =
    try
      getDeveloper ("https://en.wikipedia.org" + x)
    with
    | :? System.UriFormatException -> ""

let toString x = List.fold (fun str x -> str + x.ToString()) "" x

let refactor (s : string) = 
    let explode (s:string) =
        [for c in s -> c]
    let spaceRemover = function
        |x::xs when x = ' ' -> xs
        |x-> x
    let rec spaceAndRemover (d_list : char list list) (cur_list : char list) (s: char list) : (char list list) = 
        match s with
        |[] -> (List.rev cur_list)::d_list
        |a::b::x::y::z::w::xs when (b = ' ' && x = 'a' && y = 'n' && z = 'd' && w = ' ') ->
            spaceAndRemover ((List.rev (a::cur_list))::d_list) [] xs
        |x::xs -> spaceAndRemover d_list (x::cur_list) xs
    s |> explode  |> (spaceAndRemover [] []) |> List.map spaceRemover
    |> List.map (fun x -> toString x)


let getDevelopersList() =
    let w = getLinks()
    w 
    |> List.map (fun x -> getDeveloperSafe x)
    |> List.collect (fun x-> x.Split ',' |> Array.toList)
    |> List.collect (fun x-> refactor x)
    |> List.filter (fun x -> String.length x > 0)
    |> List.countBy (fun x -> x)
    |> List.sortBy (fun (x,y) -> y)


let getBestDeveloper() =
    let res = getDevelopersList()
    let n = List.length res
    List.item (n - 1) res

getBestDeveloper().ToString()
//result = ("John Backus", 7)

let main () = 
  let values = new NameValueCollection()
  values.Add("email", email)
  values.Add("result", getBestDeveloper().ToString())
  values.Add("content", File.ReadAllText(__SOURCE_DIRECTORY__ + @"/" + __SOURCE_FILE__))

  let client = new WebClient()
  let response = client.UploadValues(new Uri("http://91.239.143.158:13666/lab3"), values)
  let responseString = Text.Encoding.Default.GetString(response)

  printf "%A\n" responseString

main()



